#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>





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
  printf("║  " GB_AQUA "HTTP Client for Terminal " GB_GRAY "• " GB_GREEN "Fast " GB_GRAY "• " GB_YELLOW "Simple " GB_GRAY "• " GB_PURPLE "Powerful" GB_ORANGE "                 ║\n");
  printf("╚══════════════════════════════════════════════════════════════════════╝\n");
  printf(RESET "\n");
}



static void AI_Api_Function();


static void disableEcho() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

static void enableEcho() {
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    oldt.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}



typedef struct {
    char url[1024];
    char model_name[512];
    char api_key[512];
    double temp;
    char msg[4096];
} ApiInfo;


static void getApiInfo(ApiInfo *info);




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
static void Apicall(); // make an api call to any AI
static void postmethod();
static size_t callBack(char* chunk, size_t size, size_t num_element, void* storage_data); // data is the  storage where recived chunk after processing get stored





typedef struct storage
{

  char* memory; // it hold the adrrwss of the memoruy where data is sotred
  size_t size;
} storage;





// Escape JSON string to prevent injection
static char* escapeJsonString(const char *input) {
    size_t len = strlen(input);
    char *escaped = malloc(len * 2 + 1);
    
    if (!escaped) return NULL;
    
    int j = 0;
    for (size_t i = 0; i < len; i++) {
        switch (input[i]) {
            case '"': escaped[j++] = '\\'; escaped[j++] = '"'; break;
            case '\\': escaped[j++] = '\\'; escaped[j++] = '\\'; break;
            case '\n': escaped[j++] = '\\'; escaped[j++] = 'n'; break;
            case '\r': escaped[j++] = '\\'; escaped[j++] = 'r'; break;
            case '\t': escaped[j++] = '\\'; escaped[j++] = 't'; break;
            default: escaped[j++] = input[i];
        }
    }
    escaped[j] = '\0';
    return escaped;
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
  printf("│  " GB_BLUE "🗝️ " GB_YELLOW "3" GB_FG ". " GB_PURPLE "Ai-Request " GB_GRAY "│ " GB_FG " Make an Api call to any Ai" GB_GRAY "       │\n");
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
  storage* data = malloc(sizeof(storage)); // where all chunk will be stored

  data->memory = malloc(1); // so the memroy by defult have 1 byte of storege
  data->size = 0;

  CURL* handel = curl_easy_init(); // to send or recive all the request handel would be used\

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

  curl_easy_setopt(handel, CURLOPT_WRITEDATA, (void*)data); // since writedata by defult proivde response in file type so we do type conversion in void type
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
          break;

      case 3:
      AI_Api_Function();
                break;
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
static size_t callBack(char* chunk, size_t size, size_t num_element, void* storage_data)
{
  size_t total_chunk_size = size * num_element;

  // since the actuall memeory is of type storgae we'll cast void to storage type
  storage* data = (storage*)storage_data;
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
  char* ptr = realloc(data->memory, data->size + total_chunk_size + 1); // eariler size of memeory + recived chunk size + 1 (for null)

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
  CURL* handle;
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

  char* postdata;

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

    char* sep = strchr(entry, '=');
    if (!sep)
    {
      printf(GRV_RED "\nUse Proper formatting (key=value)" RESET);
      continue;
    }

    *sep = '\0';
    char* key = entry;
    char* value = sep + 1;

    // Encode key and value separately
    char* encoded_key = curl_easy_escape(handle, key, 0);
    if (!encoded_key)
    {
      fprintf(stderr, "Key encoding failed !!\n");
      break;
    }

    char* encoded_value = curl_easy_escape(handle, value, 0);
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
    char* temp = realloc(postdata, new_size);
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
  CURL* handel = curl_easy_init();
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
  char* jsondata = malloc(1);
  jsondata[0] = '\0';
  size_t total_size = 1;

  printf(GRV_AQUA "\n  Enter JSON data:\n" RESET);

  char line[256];
  int empty_line_count = 0;
  int line_count = 0;

  while (fgets(line, sizeof(line), stdin) != NULL) {
    // Check for empty line (just newline)
    if (strcmp(line, "\n") == 0) {
      empty_line_count++;
      if (empty_line_count >= 1 && line_count > 0) break;
      continue;
    }

    empty_line_count = 0;
    line_count++;

    size_t line_len = strlen(line);
    size_t new_total_size = total_size + line_len;

    char* temp = realloc(jsondata, new_total_size);
    if (!temp) {
      fprintf(stderr, "Memory allocation failed!\n");
      free(jsondata);
      return;
    }
    jsondata = temp;

    if (total_size == 1) { // First line
      strcpy(jsondata, line);
    }
    else {
      strcat(jsondata, line);
    }

    total_size = new_total_size;
  }

  // Remove the trailing newline if exists
  if (total_size > 1 && jsondata[total_size - 2] == '\n') {
    jsondata[total_size - 2] = '\0';
  }

  printf(GRV_AQUA "  Enter URL:\n  > " RESET);
  fgets(url, sizeof(url), stdin);
  url[strcspn(url, "\n")] = 0;

  printf(GRV_YELLOW "\n  🚀 Sending JSON request...\n" RESET);
  printf(GRV_GRAY "  JSON being sent:\n%s\n" RESET, jsondata);

  struct curl_slist* list = NULL;
  list = curl_slist_append(list, "Content-Type: application/json");
  list = curl_slist_append(list, "Accept: application/json");

  curl_easy_setopt(handel, CURLOPT_URL, url);
  curl_easy_setopt(handel, CURLOPT_HTTPHEADER, list);
  curl_easy_setopt(handel, CURLOPT_POSTFIELDS, jsondata);

  CURLcode result = curl_easy_perform(handel);

  if (result != CURLE_OK) {
    fprintf(stderr, "\nError while sending data: %s", curl_easy_strerror(result));
  }
  else {
    printf("\n Length of data sent: %zu bytes", strlen(jsondata));
    printf("\n ✅ Data sent successfully!");
    printf("\n\n Press any key to continue...");
    getchar();
  }

  free(jsondata);
  curl_slist_free_all(list);
  curl_easy_cleanup(handel);
}



static void getApiInfo(ApiInfo *info) {


  clearInputBuffer();


    printf(GRV_AQUA "Enter URL: " GRV_FG);
    fgets(info->url, sizeof(info->url), stdin);
    info->url[strcspn(info->url, "\n")] = 0;
    clearInputBuffer();
    printf(GRV_AQUA "Model name: " GRV_FG);
    fgets(info->model_name, sizeof(info->model_name), stdin);
    info->model_name[strcspn(info->model_name, "\n")] = 0;
    
    printf(GRV_AQUA "API Key (hidden): " RESET);
    disableEcho();
    fgets(info->api_key, sizeof(info->api_key), stdin);
    info->api_key[strcspn(info->api_key, "\n")] = 0;
    enableEcho();
    printf("\n");
    
    printf(GRV_AQUA "Temperature (0-1, default 0.5): " GRV_FG);
    char temp_input[10];
    fgets(temp_input, sizeof(temp_input), stdin);
    if (temp_input[0] != '\n') {
        info->temp = atof(temp_input);
        if (info->temp < 0) info->temp = 0;
        if (info->temp > 1) info->temp = 1;
    } else {
        info->temp = 0.5;
    }
    
    printf(GRV_AQUA "Message: " GRV_FG);
    fgets(info->msg, sizeof(info->msg), stdin);
    info->msg[strcspn(info->msg, "\n")] = 0;
    printf(RESET);
}

static void AI_Api_Function() {
    printf(GRV_PURPLE "\n╔════════════════════ API Configuration ═══════════════════╗\n");
    printf("║" GRV_FG "                                                       " GRV_PURPLE "║\n");
    printf("║ " GRV_FG "Enter your API details" GRV_PURPLE "                          ║\n");
    printf("║" GRV_FG "                                                       " GRV_PURPLE "║\n");
    printf("╚═════════════════════════════════════════════════════════════╝" RESET "\n\n");
 CURL *handle = curl_easy_init();
    
    if (!handle) {
        fprintf(stderr, GRV_RED "Failed to initialize CURL\n" RESET);
        exit(1);
    }
    
    ApiInfo info = {0};
    info.temp = 0.5;
    
    
    getApiInfo(&info);
    
    // Escape the message for JSON safety
    char *escaped_msg = escapeJsonString(info.msg);
    if (!escaped_msg) {
        fprintf(stderr, GRV_RED "Memory allocation failed\n" RESET);
        curl_easy_cleanup(handle);
        exit(1);
    }
    
    char apiauth[1024];
    char jsonRequest[8192];
    struct curl_slist *list = NULL;
    
    list = curl_slist_append(list, "Content-Type: application/json");
    
    printf(GRV_YELLOW "\n Detecting API provider...\n" RESET);
    
    // Anthropic
    if (strstr(info.url, "anthropic.com")) {
        printf(GRV_GREEN "✓ Anthropic API detected\n" RESET);
        snprintf(apiauth, sizeof(apiauth), "x-api-key: %s", info.api_key);
        list = curl_slist_append(list, apiauth);
        list = curl_slist_append(list, "anthropic-version: 2023-06-01");
        snprintf(jsonRequest, sizeof(jsonRequest),
            "{\"model\":\"%s\",\"max_tokens\":1024,\"temperature\":%.1f,\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}]}",
            info.model_name, info.temp, escaped_msg);
    }
    // Google Gemini
else if (strstr(info.url, "generativelanguage.googleapis.com")) {
    printf(GRV_GREEN "✓ Google Gemini API detected\n" RESET);
    char full_url[2048];

    // If URL doesn't already include key, add it
    if (strchr(info.url, '?')) {
        snprintf(full_url, sizeof(full_url), "%s&key=%s", info.url, info.api_key);
    } else {
        snprintf(full_url, sizeof(full_url), "%s?key=%s", info.url, info.api_key);
    }

    strcpy(info.url, full_url);

    printf(GRV_YELLOW "Final URL: %s\n\n" RESET, info.url); // it's to debug
    snprintf(jsonRequest, sizeof(jsonRequest),
        "{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}],"
        "\"generationConfig\":{\"temperature\":%.1f,\"maxOutputTokens\":1024}}",
        escaped_msg, info.temp);
}

    // OpenAI
    else if (strstr(info.url, "openai.com")) {
        printf(GRV_GREEN "✓ OpenAI API detected\n" RESET);
        snprintf(apiauth, sizeof(apiauth), "Authorization: Bearer %s", info.api_key);
        list = curl_slist_append(list, apiauth);
        snprintf(jsonRequest, sizeof(jsonRequest),
            "{\"model\":\"%s\",\"max_tokens\":1024,\"temperature\":%.1f,\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}]}",
            info.model_name, info.temp, escaped_msg);
    }
    // Groq
    else if (strstr(info.url, "groq.com")) {
        printf(GRV_GREEN "✓ Groq API detected\n" RESET);
        snprintf(apiauth, sizeof(apiauth), "Authorization: Bearer %s", info.api_key);
        list = curl_slist_append(list, apiauth);
        snprintf(jsonRequest, sizeof(jsonRequest),
            "{\"model\":\"%s\",\"max_tokens\":1024,\"temperature\":%.1f,\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}]}",
            info.model_name, info.temp, escaped_msg);
    }
    // Cohere
    else if (strstr(info.url, "cohere.ai")) {
        printf(GRV_GREEN "✓ Cohere API detected\n" RESET);
        snprintf(apiauth, sizeof(apiauth), "Authorization: Bearer %s", info.api_key);
        list = curl_slist_append(list, apiauth);
        snprintf(jsonRequest, sizeof(jsonRequest),
            "{\"model\":\"%s\",\"max_tokens\":1024,\"temperature\":%.1f,\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}]}",
            info.model_name, info.temp, escaped_msg);
    }
    // Mistral
    else if (strstr(info.url, "mistral.ai")) {
        printf(GRV_GREEN "✓ Mistral API detected\n" RESET);
        snprintf(apiauth, sizeof(apiauth), "Authorization: Bearer %s", info.api_key);
        list = curl_slist_append(list, apiauth);
        snprintf(jsonRequest, sizeof(jsonRequest),
            "{\"model\":\"%s\",\"max_tokens\":1024,\"temperature\":%.1f,\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}]}",
            info.model_name, info.temp, escaped_msg);
    }
    // Default (most APIs use Bearer token)
    else {
        printf(GRV_YELLOW "⚠ Using default Bearer token format\n" RESET);
        snprintf(apiauth, sizeof(apiauth), "Authorization: Bearer %s", info.api_key);
        list = curl_slist_append(list, apiauth);
        snprintf(jsonRequest, sizeof(jsonRequest),
            "{\"model\":\"%s\",\"max_tokens\":1024,\"temperature\":%.1f,\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}]}",
            info.model_name, info.temp, escaped_msg);
    }
    
    // Prepare response buffer
  storage response = {0};
    response.memory = malloc(1);
    if (!response.memory) {
        fprintf(stderr, GRV_RED "Memory allocation failed\n" RESET);
        curl_easy_cleanup(handle);
        exit(1);
    }
    
    // Set CURL options
    curl_easy_setopt(handle, CURLOPT_URL, info.url);
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt(handle, CURLOPT_POSTFIELDS, jsonRequest);
    curl_easy_setopt(handle, CURLOPT_ENCODING," ");
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callBack);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&response);
    
    // Handle compression automatically
    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");
    
    printf(GRV_YELLOW "📤 Sending request...\n" RESET);
    
    CURLcode result = curl_easy_perform(handle);
    
    if (result != CURLE_OK) {
        fprintf(stderr, GRV_RED "\n❌ Error: %s\n" RESET, curl_easy_strerror(result));
    } else {
        printf(GRV_GREEN "\n✅ Request successful!\n" RESET);
        printf(GRV_YELLOW "Bytes sent: %zu\n\n" RESET, strlen(jsonRequest));
        printf(GRV_AQUA "═══════════════════ Response ═══════════════════\n");
        printf(GRV_FG "%s\n" RESET, response.memory);
        printf(GRV_AQUA "═══════════════════════════════════════════════════\n" RESET);
    }
    
    free(escaped_msg);
    free(response.memory);
    curl_slist_free_all(list);
    curl_easy_cleanup(handle);
 

printf(GRV_GRAY "\nPress Enter to return to main menu..." RESET);
getchar(); 


}