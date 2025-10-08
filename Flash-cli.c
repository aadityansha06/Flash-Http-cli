

/// After any modfication made please change the version.h file

/*
version number is divided into 3 parts, minor, major and patch
if a fix is small that it doesnt break anything, like formatting, then we update patch number
like 1.0.1 -> 1.0.2
1.0.0 -
1 - major
2-minor
3-patch bugs etc



*/

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ui/colors.h"
#include <sys/stat.h>

#include "version.h"

#define REPO "aadityansha06/Flash-Http-cli"
#define LINUX_BIN_NAME "flash-linux-x64"
#define BAK "flash.bak"
#define TMP "flash.tmp"

static void ContentApplication(); ///  content-type  application/x-www-form-urlencoded  data
static void ContentJson();        // sending content type json

static void postmethod();
static size_t callBack(char *chunk, size_t size, size_t num_element, void *storage_data); // data is the  storage where recived chunk after processing get stored

static int download_file(const char *url, const char *outfile);
static int get_latest_tag(char *tag, size_t maxlen);
static void self_update(const char *download_url);
typedef struct storage
{

  char *memory; // it hold the adrrwss of the memoruy where data is sotred
  size_t size;
} storage;

int num = 1;
static size_t callBack(char *chunk, size_t size, size_t num_element, void *storage_data)
{
  size_t total_chunk_size = size * num_element;

  // since the actuall memeory is of type storgae we'll cast void to storage type
  storage *data = (storage *)storage_data;
  //  printf("%d chunk(%zu byte  size) : \n \n %.*s",num, total_chunk_size ,(int )total_chunk_size,chunk);
  //  fflush(stdout);
  // i++;

  /*

  %zu → print an unsigned size_t (total_chunk_size, the chunk length in bytes).

  %.*s → this is the tricky one: !!! no space btw . and *

  %s normally prints a string until it finds a null terminator (\0).

  %.*s means: print exactly N characters from the string, even if there's no \0.

  The * is replaced by the integer you pass in the argument list.

  So %.*s takes two arguments:

  the maximum number of characters to print (here (int)total_chunk_size)

  the pointer to the data (here chunk)

  */

  // since we have total chunk size we can increase the size of storage using realloc
  char *ptr = realloc(data->memory, data->size + total_chunk_size + 1); // eariler size of memeory + recived chunk size + 1 (for null)

  if (ptr == NULL)
  {
    printf(" \t Unbale to realloc the size");
    return 0;
  }

  data->memory = ptr; // Why? , now it'll hold the address of memory whose size is
  /// eariler size of memeory + recived chunk size + 1

  memcpy(&(data->memory[data->size]), chunk, total_chunk_size); // if the data size is 0 , then from meemroy[0] the chunk will be added to meemory
  // and subsequently it will be upadted based on current chunk recived with   mem->size +=totla_chunk_size;

  data->size += total_chunk_size;

  data->memory[data->size] = 0; // null at the end of each chunk recive to avoid garage value

  return total_chunk_size; // returning this would help libcurl to comapre that the chunk size it sent was exactuly the same we recived
}

int download_file(const char *url, const char *outfile)
{
  CURL *curl;
  FILE *fp;
  CURLcode res;

  fp = fopen(outfile, "wb");
  if (!fp)
  {
    perror("fopen");
    return 1;
  }

  curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/8.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    fclose(fp);
    return (res == CURLE_OK) ? 0 : 1;
  }

  fclose(fp);
  fprintf(stderr, "curl init failed\n");
  return 1;
}

// Fetch latest tag from GitHub
int get_latest_tag(char *tag, size_t maxlen)
{
  CURL *curl;
  CURLcode res;
  struct storage chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  curl = curl_easy_init();
  if (!curl)
    return 1;

  char url[256];
  snprintf(url, sizeof(url), "https://api.github.com/repos/%s/releases/latest", REPO);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "flash-cli-updater");
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBack);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK)
  {
    free(chunk.memory);
    return 1;
  }

  // --- Extract "tag_name" ---
  char *p = strstr(chunk.memory, "\"tag_name\":\"");
  if (!p)
  {
    free(chunk.memory);
    return 1;
  }
  p += strlen("\"tag_name\":\"");
  char *q = strchr(p, '"');
  if (!q)
  {
    free(chunk.memory);
    return 1;
  }
  size_t len = q - p;
  if (len >= maxlen)
    len = maxlen - 1;
  strncpy(tag, p, len);
  tag[len] = 0;

  // --- Extract and show "name" (release title) ---
  char release_name[256] = "N/A";
  char *n = strstr(chunk.memory, "\"name\":\"");
  if (n)
  {
    n += strlen("\"name\":\"");
    char *m = strchr(n, '"');
    if (m)
    {
      size_t name_len = m - n;
      if (name_len >= sizeof(release_name))
        name_len = sizeof(release_name) - 1;
      strncpy(release_name, n, name_len);
      release_name[name_len] = 0;
    }
  }

  printf("Latest version: %s\n", tag);
  printf("Release name: %s\n", release_name);

  free(chunk.memory);
  return 0;
}

void self_update(const char *download_url)
{
  printf("Downloading new version...\n");
  if (download_file(download_url, TMP) != 0)
  {
    printf("Download failed");
    return;
  }

  printf("Starting Updater...\n");
  pid_t pid = fork();
  if (pid == 0)
  {
    sleep(1);
    rename("./flash", BAK);
    rename(TMP, "./flash");
    chmod("./flash", 0755);
    execl("./flash", "./flash", NULL);
    exit(0);
  }
  else
  {
    exit(0);
  }
}

// Beautiful Flash header
void printFlashHeader()
{
  printf(GB_ORANGE BOLD);
  printf("╔══════════════════════════════════════════════════════════════════════╗\n");
  printf("║  " GB_BG_BLUE GB_FG "                              FLASH                               " RESET GB_ORANGE BOLD "  ║\n");
  printf("║  " GB_AQUA "HTTP Client for Terminal " GB_GRAY "• " GB_GREEN "Fast " GB_GRAY "• " GB_YELLOW "Simple " GB_GRAY "• " GB_PURPLE "Powerful" GB_ORANGE "                 ║\n");
  printf("╚══════════════════════════════════════════════════════════════════════╝\n");
  printf(RESET "\n");
}
void clearInputBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

void printSeparator()
{
  printf(GRV_GRAY "  ═════════════════════════════════════════════════════\n" RESET);
}

int display()
{
  int opt = -1;
  system("clear");

  printFlashHeader();

  // Menu options with beautiful formatting
  printf(GB_FG "┌─────────────────────────────────────────────────────────────────────┐\n");
  printf("│                                                                     │\n");
  printf("│  " GB_GREEN "⚡ " GB_YELLOW "1" GB_FG ". " GB_AQUA "GET Request   " GB_GRAY "│ " GB_FG "Retrieve data from endpoint            " GB_GRAY "      │\n");
  printf("│  " GB_ORANGE "📡 " GB_YELLOW "2" GB_FG ". " GB_PURPLE "POST Request  " GB_GRAY "│ " GB_FG "Send data to server                   " GB_GRAY "       │\n");
  printf("│  " GB_RED "🚪 " GB_YELLOW "0" GB_FG ". " GB_RED "Exit          " GB_GRAY "│ " GB_FG "Close Flash CLI                       " GB_GRAY "       │\n");
  printf("│                                                                     │\n");
  printf("└─────────────────────────────────────────────────────────────────────┘\n" RESET);

  printf("\n" GB_AQUA "❯ " GB_FG "Enter your choice: " GB_YELLOW);
  scanf("%d", &opt);
  printf(RESET);

  return opt;
}

void geturl()
{
  storage *data = malloc(sizeof(storage)); // where all chunk will be stored

  data->memory = malloc(1); // so the memroy by defult have 1 byte of storege
  data->size = 0;

  CURL *handel = curl_easy_init(); // to send or recive all the request handel would be used\

  if (handel == NULL)
  {
    fprintf(stderr, "Unable to initilize curl \n");
  }
  char url[400];

  printf("\nEnter the url:");
  scanf("%s", url);

  // Clear the input buffer after scanf
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;

  curl_easy_setopt(handel, CURLOPT_URL, url); /*  setting up the endpoint
                                                             where request need to be send   */

  curl_easy_setopt(handel, CURLOPT_WRITEFUNCTION, callBack); // function to handel the chuck data recived

  curl_easy_setopt(handel, CURLOPT_WRITEDATA, (void *)data); // since writedata by defult proivde response in file type so we do type conversion in void type
  // void type conversion whill free us to convert the datinto any other type as per need

  CURLcode result = curl_easy_perform(handel); // it will perform the task of sending request
  // result wil store the returned value or status code
  // 0 means ok else means there's an error

  if (result != CURLE_OK)
  {
    fprintf(stderr, "Error: %s", curl_easy_strerror(result));
  }
  else
  {
    printf("\n\n----------Response--------\n\n %s\n", data->memory);
  }

  curl_easy_cleanup(handel);
  free(data->memory);
  free(data);
}

int main()
{

  curl_global_init(CURL_GLOBAL_DEFAULT); // moved earlier

  // How auto update working

  // making a call to https://api.github.com/repos/aadityansha06/Flash-Http-cli/releases/latest
  /*
  getting a json response about latest verison
  ex -
  ,
    "tag_name": "1.0.2",
    "target_commitish": "main",
    "name": "Enhanced JSON input with multi-line support and improved UI alignment",
        "url": "https://api.github.com/repos/aadityansha06/Flash-Http-cli/releases/assets/3590",


  }

  Then praising the tag name and comparing it with the verison.h verison if it's npt same then echoing it's an new update




  */

  if (getenv("FLASH_SKIP_UPDATE") == NULL)
  { // skip if just updated
    char latest[64];
    const char *base_version = RELEASE_VERSION;

    // printf("Current version (compiled): %s\n", RELEASE_VERSION);
    printf("New version available!\n");
    printf("Installed version record : %s\n", base_version);

    if (get_latest_tag(latest, sizeof(latest)) != 0)
    {
      printf("Failed to fetch latest version.\n");
    }
    else
    {

      if (strcmp(latest, base_version) != 0)
      {
        // only prompt if remote differs from installed record

        printf("\nDo you want to update (y/n): ");
        char ans = 'n';
        if (scanf(" %c", &ans) != 1)
          ans = 'n';
        int flush_ch;
        while ((flush_ch = getchar()) != '\n' && flush_ch != EOF)
        {
        }
        if (ans == 'y' || ans == 'Y')
        {
          char download_url[256];
          snprintf(download_url, sizeof(download_url),
                   "https://github.com/%s/releases/download/%s/%s",
                   REPO, latest, LINUX_BIN_NAME);
          printf("Downloading new version (%s)...\n", latest);
          if (download_file(download_url, TMP) == 0)
          {
            if (rename("./flash", BAK) != 0)
              perror("backup failed");
            if (rename(TMP, "./flash") != 0)
            {
              perror("replace failed");
              rename(BAK, "./flash");
            }
            else
            {
              chmod("./flash", 0755);
              printf("Updated to %s. Restarting...\n", latest);
              setenv("FLASH_SKIP_UPDATE", "1", 1); // guard to prevent loop
              execl("./flash", "./flash", NULL);
              perror("execl failed");
            }
          }
          else
          {
            printf("Download failed.\n");
          }
        }
      }
      else
      {
        printf("Already up to date.\n");
      }
    }
  }
  else
  {
    printf("Skipping update check (FLASH_SKIP_UPDATE set).\n");
  }

  int opt = -1;
  char ch;

  do
  {
    opt = display();

    switch (opt)
    {
    case 0:
      break;

    case 1:
      geturl();
      printf("\n\n Press any key to exit ....");
      getchar();

      break;

    case 2:
      postmethod();

    default:
      printf("\n !!! Please make a vaild choice \n");
      break;
    }

  } while (opt != 0);

  if (opt == 0)
  {
    printf(GRV_GREEN "\n  👋 Thank you for using Flash CLI! BY bY ... \n" RESET);
  }
  curl_global_cleanup();

  return 0;
}

static void postmethod()
{

  int opt = -1;

  //   curl_global_init(CURL_GLOBAL_DEFAULT);
  do
  {
    system("clear");

    printf(GRV_PURPLE "  ┌─────────────────────────────────────────────────────┐\n");
    printf("  │" GRV_FG "                  " BOLD "POST METHOD" RESET GRV_FG "                   " GRV_PURPLE "     │\n");
    printf("  ├─────────────────────────────────────────────────────┤\n");
    printf("  │                                                     │\n");
    printf("  │  " GRV_YELLOW "📋 " GRV_BLUE "1." GRV_FG " Form Data    " GRV_GRAY "- application/x-www-form     " GRV_PURPLE "   │\n");
    printf("  │                    " GRV_GRAY "  urlencoded                  " GRV_PURPLE "   │\n");
    printf("  │                                                     │\n");
    printf("  │  " GRV_GREEN "📄 " GRV_BLUE "2." GRV_FG " JSON Data    " GRV_GRAY "- application/json           " GRV_PURPLE "   │\n");
    printf("  │                                                     │\n");
    printf("  │  " GRV_RED "🔙 " GRV_BLUE "0." GRV_FG " Back         " GRV_GRAY "- Return to main menu        " GRV_PURPLE "   │\n");
    printf("  │                                                     │\n");
    printf("  └─────────────────────────────────────────────────────┘\n");
    printf(RESET);

    printf(GRV_AQUA "\n  ➤ " GRV_FG "Choose content type: " RESET);
    scanf("%d", &opt);
    int c;

    switch (opt)
    {
    case 0:

      return;

    case 1:
      ContentApplication(); ///  content-type  application/x-www-form-urlencoded  data

      break;

    case 2:
      ContentJson(); // sesending contetn type json

    default:

      printf("\n !!! Please make a vaild choice \n");
      break;
    }

  } while (opt != 0);
}

static void ContentApplication()
{
  CURL *handle;
  handle = curl_easy_init();

  system("clear");

  printf(GRV_YELLOW "  ┌─────────────────────────────────────────────────────┐\n");
  printf("  │" GRV_FG "                " BOLD "FORM DATA INPUT" RESET GRV_FG "                " GRV_YELLOW "      │\n");
  printf("  └─────────────────────────────────────────────────────┘\n" RESET);

  ;

  printf(GRV_GRAY "\n  Rules:\n");
  printf("  • Key-value pairs separated by " GRV_YELLOW "'=' "
         "\neg.\nid=1\n name=jhon\n status=active\n" RESET);
  printf(GRV_RED "• Use Proper Formatting\n" RESET);

  char *postdata;

  printSeparator();
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
  int num_fields = 0;

  while (1)
  {
    printf(GRV_AQUA "\nEnter number of fields: " RESET);
    if (scanf("%d", &num_fields) != 1)
    {
      printf(GRV_RED "\nEnter only a number!!" RESET);

      // clear stdin buffer
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      continue;
    }
    if (num_fields <= 0)
    {
      printf(GRV_RED "\nEnter a positive number!!" RESET);
      continue;
    }
    break;
  }

  while ((c = getchar()) != '\n' && c != EOF)
    ;

  char entry[256];

  // Initialize postdata
  postdata = malloc(1);
  postdata[0] = '\0';

  int n = num_fields;
  while (n)
  {
    printf(GRV_AQUA "Enter field %d (key=value) : ", num_fields - n + 1);
    fgets(entry, sizeof(entry), stdin);
    entry[strcspn(entry, "\n")] = 0;
    if (strlen(entry) == 0)
    {
      printf(GRV_RED "\nEmpty input! Please try again." RESET);
      continue;
    }

    char *sep = strchr(entry, '=');
    if (!sep)
    {
      printf(GRV_RED "\nUse Proper formatting (key=value)" RESET);
      continue;
    }

    *sep = '\0';
    char *key = entry;
    char *value = sep + 1;

    // Encode key and value separately
    char *encoded_key = curl_easy_escape(handle, key, 0);
    if (!encoded_key)
    {
      fprintf(stderr, "Key encoding failed !!\n");
      break;
    }

    char *encoded_value = curl_easy_escape(handle, value, 0);
    if (!encoded_value)
    {
      fprintf(stderr, "Value encoding failed !!\n");
      curl_free(encoded_key);
      break;
    }

    // Calculate size needed for "key=value"
    size_t encoded_pair_len = strlen(encoded_key) + strlen(encoded_value) + 1; // +1 for '='
    size_t current_len = strlen(postdata);
    size_t new_size = current_len + encoded_pair_len + 2; // +1 for '&', +1 for '\0'

    // Reallocate memory
    char *temp = realloc(postdata, new_size);
    if (!temp)
    {
      fprintf(stderr, "Memory allocation failed!\n");
      curl_free(encoded_key);
      curl_free(encoded_value);
      break;
    }
    postdata = temp;

    // Add separator if not first field
    if (current_len > 0)
    {
      strcat(postdata, "&");
    }

    // Append encoded key=value pair
    strcat(postdata, encoded_key);
    strcat(postdata, "=");
    strcat(postdata, encoded_value);

    //

    // Free the encoded value
    curl_free(encoded_key);
    curl_free(encoded_value);

    n--;
  }

  printf("%s\n", postdata);

  char url[400];

  printf(GRV_AQUA "  Enter URL: " RESET);
  scanf("%s", url);
  printf(GRV_YELLOW "\n   Sending request...\n" RESET);

  curl_easy_setopt(handle, CURLOPT_URL, url);

  curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postdata);

  CURLcode result = curl_easy_perform(handle);

  if (result != CURLE_OK)
  {
    fprintf(stderr, "Eroor while posting data : %s", curl_easy_strerror(result));
    exit(1);
  }
  else
  {
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    printf("\n Length of data sent %zu bytes ", (size_t)strlen(postdata));
    printf("\n !!Data sent sucessfully ..");
    printf("\n\n Press any key to exit ....");
    getchar();
  }

  curl_easy_cleanup(handle);
}

static void ContentJson()
{
  CURL *handel = curl_easy_init();
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;

  char url[1024];
  printf("\n");

  printf(GRV_GREEN "  ┌─────────────────────────────────────────────────────┐\n");
  printf("  │" GRV_FG "                " BOLD "JSON DATA INPUT" RESET GRV_FG "                 " GRV_GREEN "│\n");
  printf("  └─────────────────────────────────────────────────────┘\n" RESET);

  printf(GRV_GRAY "\n  JSON format example:\n");
  printf(GRV_AQUA "  {\n");
  printf("    \"id\": 21,\n");
  printf("    \"name\": \"Flash User\",\n");
  printf("    \"email\": \"user@example.com\",\n");
  printf("    \"active\": true\n");
  printf("  }\n" RESET);

  printf(GRV_GRAY "\n  Instructions:\n");
  printf("  • Enter JSON data line by line\n");
  printf("  • Press " GRV_YELLOW "Enter twice" GRV_GRAY " to finish input\n");
  printSeparator();

  // Dynamic string for JSON data
  char *jsondata = malloc(1);
  jsondata[0] = '\0';
  size_t total_size = 1;

  printf(GRV_AQUA "\n  Enter JSON data:\n" RESET);

  char line[256];
  int empty_line_count = 0;
  int line_count = 0;

  while (fgets(line, sizeof(line), stdin) != NULL)
  {
    // Check for empty line (just newline)
    if (strcmp(line, "\n") == 0)
    {
      empty_line_count++;
      if (empty_line_count >= 1 && line_count > 0)
        break;
      continue;
    }

    empty_line_count = 0;
    line_count++;

    size_t line_len = strlen(line);
    size_t new_total_size = total_size + line_len;

    char *temp = realloc(jsondata, new_total_size);
    if (!temp)
    {
      fprintf(stderr, "Memory allocation failed!\n");
      free(jsondata);
      return;
    }
    jsondata = temp;

    if (total_size == 1)
    { // First line
      strcpy(jsondata, line);
    }
    else
    {
      strcat(jsondata, line);
    }

    total_size = new_total_size;
  }

  // Remove the trailing newline if exists
  if (total_size > 1 && jsondata[total_size - 2] == '\n')
  {
    jsondata[total_size - 2] = '\0';
  }

  printf(GRV_AQUA "  Enter URL:\n  > " RESET);
  fgets(url, sizeof(url), stdin);
  url[strcspn(url, "\n")] = 0;

  printf(GRV_YELLOW "\n  🚀 Sending JSON request...\n" RESET);
  printf(GRV_GRAY "  JSON being sent:\n%s\n" RESET, jsondata);

  struct curl_slist *list = NULL;
  list = curl_slist_append(list, "Content-Type: application/json");
  list = curl_slist_append(list, "Accept: application/json");

  curl_easy_setopt(handel, CURLOPT_URL, url);
  curl_easy_setopt(handel, CURLOPT_HTTPHEADER, list);
  curl_easy_setopt(handel, CURLOPT_POSTFIELDS, jsondata);

  CURLcode result = curl_easy_perform(handel);

  if (result != CURLE_OK)
  {
    fprintf(stderr, "\nError while sending data: %s", curl_easy_strerror(result));
  }
  else
  {
    printf("\n Length of data sent: %zu bytes", strlen(jsondata));
    printf("\n ✅ Data sent successfully!");
    printf("\n\n Press any key to continue...");
    getchar();
  }

  free(jsondata);
  curl_slist_free_all(list);
  curl_easy_cleanup(handel);
}
