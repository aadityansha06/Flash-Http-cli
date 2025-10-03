#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define DIM "\033[2m"

// Gruvbox colors
#define GB_BG "\033[48;2;40;40;40m"        // Dark background
#define GB_FG "\033[38;2;235;219;178m"     // Light foreground
#define GB_RED "\033[38;2;251;73;52m"      // Bright red
#define GB_GREEN "\033[38;2;184;187;38m"   // Bright green
#define GB_YELLOW "\033[38;2;250;189;47m"  // Bright yellow
#define GB_BLUE "\033[38;2;131;165;152m"   // Bright blue
#define GB_PURPLE "\033[38;2;211;134;155m" // Bright purple
#define GB_AQUA "\033[38;2;142;192;124m"   // Bright aqua
#define GB_ORANGE "\033[38;2;254;128;25m"  // Bright orange
#define GB_GRAY "\033[38;2;168;153;132m"   // Gray

#define GRV_BG "\033[48;2;40;40;40m"        // Dark background
#define GRV_FG "\033[38;2;235;219;178m"     // Light foreground
#define GRV_RED "\033[38;2;251;73;52m"      // Bright red
#define GRV_GREEN "\033[38;2;184;187;38m"   // Bright green
#define GRV_YELLOW "\033[38;2;250;189;47m"  // Bright yellow
#define GRV_BLUE "\033[38;2;131;165;152m"   // Bright blue
#define GRV_PURPLE "\033[38;2;211;134;155m" // Bright purple
#define GRV_AQUA "\033[38;2;142;192;124m"   // Bright aqua
#define GRV_ORANGE "\033[38;2;254;128;25m"  // Bright orange
#define GRV_GRAY "\033[38;2;168;153;132m"
// Background variants
#define GB_BG_RED "\033[48;2;251;73;52m"
#define GB_BG_GREEN "\033[48;2;184;187;38m"
#define GB_BG_BLUE "\033[48;2;131;165;152m"
// Beautiful Flash header
void printFlashHeader()
{
  printf(GB_ORANGE BOLD);
  printf("╔══════════════════════════════════════════════════════════════════════╗\n");
  printf("║  " GB_BG_BLUE GB_FG "                              FLASH                               " RESET GB_ORANGE BOLD "  ║\n");
  printf("║  " GB_AQUA "HTTP Client for Terminal " GB_GRAY "• " GB_GREEN "Fast " GB_GRAY "• " GB_YELLOW "Simple " GB_GRAY "• " GB_PURPLE "Powerful" GB_ORANGE "     ║\n");
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

static void ContentApplication(); ///  content-type  application/x-www-form-urlencoded  data
static void ContentJson();        // sending content type json

static void postmethod();
static size_t callBack(char *chunk, size_t size, size_t num_element, void *storage_data); // data is the  storage where recived chunk after processing get stored

typedef struct storage
{

  char *memory; // it hold the adrrwss of the memoruy where data is sotred
  size_t size;
} storage;

int display()
{
  int opt = -1;
  system("clear");

  printFlashHeader();

  // Menu options with beautiful formatting
  printf(GB_FG "┌─────────────────────────────────────────────────────────────────────┐\n");
  printf("│                                                                     │\n");
  printf("│  " GB_GREEN "⚡ " GB_YELLOW "1" GB_FG ". " GB_AQUA "GET Request   " GB_GRAY "│ " GB_FG "Retrieve data from endpoint            " GB_GRAY "│\n");
  printf("│  " GB_ORANGE "📡 " GB_YELLOW "2" GB_FG ". " GB_PURPLE "POST Request  " GB_GRAY "│ " GB_FG "Send data to server                   " GB_GRAY "│\n");
  printf("│  " GB_RED "🚪 " GB_YELLOW "0" GB_FG ". " GB_RED "Exit          " GB_GRAY "│ " GB_FG "Close Flash CLI                       " GB_GRAY "│\n");
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
  curl_global_init(CURL_GLOBAL_DEFAULT);
  int opt = -1;
  char ch;
label:
  //  system("clear");

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

static void postmethod()
{

  int opt = -1;

  //   curl_global_init(CURL_GLOBAL_DEFAULT);
  do
  {
    system("clear");

    printf(GRV_PURPLE "  ┌─────────────────────────────────────────────────────┐\n");
    printf("  │" GRV_FG "                  " BOLD "POST METHOD" RESET GRV_FG "                   " GRV_PURPLE "│\n");
    printf("  ├─────────────────────────────────────────────────────┤\n");
    printf("  │                                                     │\n");
    printf("  │  " GRV_YELLOW "📋 " GRV_BLUE "1." GRV_FG " Form Data    " GRV_GRAY "- application/x-www-form     " GRV_PURPLE "│\n");
    printf("  │                    " GRV_GRAY "  urlencoded                  " GRV_PURPLE "│\n");
    printf("  │                                                     │\n");
    printf("  │  " GRV_GREEN "📄 " GRV_BLUE "2." GRV_FG " JSON Data    " GRV_GRAY "- application/json           " GRV_PURPLE "│\n");
    printf("  │                                                     │\n");
    printf("  │  " GRV_RED "🔙 " GRV_BLUE "0." GRV_FG " Back         " GRV_GRAY "- Return to main menu        " GRV_PURPLE "│\n");
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
         "\neg.\nid=1\n name=jhon\n status=active\n"RESET);
  printf(GRV_RED"• Use Proper Formatting\n" RESET);

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
  char jsondata[1024];
  char url[1024];
  printf("\n");

  printf(GRV_GREEN "  ┌─────────────────────────────────────────────────────┐\n");
  printf("  │" GRV_FG "                " BOLD "JSON DATA INPUT" RESET GRV_FG "                 " GRV_GREEN "│\n");
  printf("  └─────────────────────────────────────────────────────┘\n" RESET);

  printf(GRV_GRAY "\n  JSON format example:\n");
  printf(GRV_AQUA "  {\n");
  printf("    \"id\": 21,");
  printf("    \"name\": \"Flash User\",");
  printf("    \"email\": \"user@example.com\",");
  printf("    \"active\": true");
  printf("  }\n" RESET);

  printf(GRV_AQUA "\n  Enter JSON data:\n  > " RESET);
  fgets(jsondata, sizeof(jsondata), stdin);
  jsondata[strcspn(jsondata, "\n")] = 0; // remove newline

  printf(GRV_AQUA "   Enter URL:\n  > " RESET);
  fgets(url, sizeof(url), stdin);
  url[strcspn(url, "\n")] = 0; // remove newline
  printf(GRV_YELLOW "\n  🚀 Sending JSON request...\n" RESET);
  struct curl_slist *list = NULL; // creating a new list

  list = curl_slist_append(list, "Content-Type: application/json");
  list = curl_slist_append(list, "Accept: application/json ");

  // this is our custom header linkid - list  which will be passed through  CURLOPT_HTTPHEADER

  curl_easy_setopt(handel, CURLOPT_URL, url);         // setting url endpoint
  curl_easy_setopt(handel, CURLOPT_HTTPHEADER, list); // this willl prepare to send the custom header

  curl_easy_setopt(handel, CURLOPT_POSTFIELDS, jsondata); // this willl prepare to send the json data

  CURLcode result = curl_easy_perform(handel);

  if (result != CURLE_OK)
  {
    fprintf(stderr, "\nError while sending data %s", curl_easy_strerror(result));
  }
  else
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;

    printf("\n Length of data sent %zu bytes ", (size_t)strlen(jsondata));
    printf("\n !!Data sent sucessfully ..");
    printf("\n\n Press any key to exit ....");
    getchar();
  }

  curl_slist_free_all(list);
  curl_easy_cleanup(handel);
}