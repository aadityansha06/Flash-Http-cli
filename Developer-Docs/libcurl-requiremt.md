 
****CURLcode curl_global_init(long flags); *******

// CURLcode is struct 
// flag tell libcurl which feature to init 
// By defalut we use CURL_GLOBAL_ALL(Initialize everything possible. )
```
int main(void)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
 
  /* use libcurl, then before exiting... */
 
  curl_global_cleanup();
}
```


while  curl_global_init() help to load all libcrul function 

This function sets up the program environment that libcurl needs. Think of it as an extension of the library loader.

libcurl has two interface 
1. easy interface ( whcih is shryconhys )

2. multi interface (asynchronoyus)

*** CURL *curlhandel = curl_easy_init(); ***
This function allocates and returns an easy handle. Such a handle is used as input to other functions in the easy interface

if(curl==NUll)->something went wrong

CURL *curl = curl_easy_init(); returns an easy handle (stored in *curlhandel). Using that we can proceed to the next step: setting up  preferred actions which we want to pertform



****curl_easy_cleanup***
This function is the opposite of curl_easy_init. It closes down and frees all resources previously associated with this easy handle.



****CURLcode curl_easy_setopt(CURL *handle, CURLoption option, parameter);****

curl_easy_setopt is used to tell libcurl how to behave. By setting the appropriate options, the application can change libcurl's behavior. All options are set with an option followed by a parameter.

<b> it just prepare or steup the request which is to be send , it doesn't actually send the request tos server, the request is send by 
curl_easy_perform(curlhandel) about which is discussed below  </b>


<hr></hr>
🟢 The Most Important CURLoption (Beginner level)
1. CURLOPT_URL

👉 Sets which website (or API endpoint) you want to talk to.

curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");


Without this, libcurl doesn’t know where to connect.

2. CURLOPT_WRITEFUNCTION

👉 Tells libcurl: “When the response arrives, call this function to handle it.”

curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_callback_function);


Your callback decides what to do with the response (print, save, store in memory).

3. CURLOPT_WRITEDATA

👉 Gives your callback a place/memory to store the recived chunk of data( by default it store the recived chunk in file fopen("recive.txt","wb")  ) .

curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&myStruct); // type conversion is needed if using custom callback if we want to store data in memeory rahter than file




Ex->
struct MemoryData { // palce whwere whole data will be stored
    char *memory;
    size_t size;
}



  // Set our custom write function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToMemory);
        

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response); // it tells the callback where to put the recived chunk of data (i.e put chunk into data->memory)

chunk is the packet what we keep reciving using writefunction and data->memory is the place where all the chunk get sotred together using wirtedata


📡 INTERNET DATA STREAM
     │
     ▼
┌─────────────────┐
│ chunk 1: "Hel"  │ ──┐
└─────────────────┘   │
┌─────────────────┐   │    📝 WRITEFUNCTION
│ chunk 2: "lo "  │ ──┼──► WriteToMemory()
└─────────────────┘   │    (processes each chunk)
┌─────────────────┐   │
│ chunk 3: "Wor"  │ ──┤
└─────────────────┘   │
┌─────────────────┐   │
│ chunk 4: "ld!"  │ ──┘
└─────────────────┘
                      │
                      ▼
            ┌──────────────────────┐
            │   data->memory       │ ◄── 📦 WRITEDATA
            │ "Hello World!"       │     (storage location)
            └──────────────────────┘


            chunk = Each small packet of data arriving
WRITEFUNCTION = Takes each chunk and processes it
data->memory = The big storage box where all chunks get combined








// This function gets called for each chunk of data (since we don't get all data at a time)


// Think of it as: "Hey, here's another piece of data, what do you want to do with it?"


size_t WriteToMemory(void *chunk, size_t size, size_t nmemb, struct MemoryData *data) {
    size_t chunk_size = size * nmemb;  // Total bytes in this chunk
    
    printf(" Received chunk of %zu bytes\n", chunk_size);
    
    // Resize our memory to fit the new data
    char *ptr = realloc(data->memory, data->size + chunk_size + 1);
    if(!ptr) {
        printf(" Out of memory!\n");
        return 0;  // Tell curl we failed
    }
    
    data->memory = ptr;
    
    // Copy the new chunk to our memory
    memcpy(&(data->memory[data->size]), chunk, chunk_size);
    data->size += chunk_size;
    data->memory[data->size] = 0;  // Null terminator
    
    return chunk_size;  // Tell curl we handled all the data
}
Suppose mem->size = 6 (we already stored "Hello "), and realsize = 6 ("World!")
if we update first:

mem->size += realsize;  // mem->size = 12
memcpy(&(mem->memory[mem->size]), contents, realsize);


Now the memcpy writes at offset 12, skipping over the existing data’s end (offset 6).
That leaves a gap of 6 uninitialized bytes between "Hello " and "World!".








If you don’t care, libcurl prints directly to stdout by default.

If you set this, it passes your variable (like a struct or file pointer) into your callback.

4. CURLOPT_POSTFIELDS (only for POST requests)

👉 If you want to send data to the server (like submitting a form or sending JSON), you use this.

curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=John&age=25");


or

curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"key\":\"value\"}");




erything we set with curl_easy_setopt() is just preparing the request — but nothing is actually sent yet.

That’s where curl_easy_perform() comes in.

🔹 What curl_easy_perform() does

It takes your CURL *handle (with all the options you set)

Actually sends the HTTP request to the server

Waits for the response

Runs your callback function(s) when data arrives (if callback is set)

Returns a status code (success or error) 

Ex-  curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
    res = curl_easy_perform(curl);
!!! Result willl be of type CURLcode
    status code is stored inside result 

    CURLE_OK (0) means everything was OK, non-zero means an error occurred,

    which can be print using 
//stderr to output error
// stdout for normal output

    if(res!=CUrlOK){
     curl_easy_strerror can be called to get an error string from a given CURLcode number.
          fprintf(stderr, "Error: %s\n",curl_easy_strerror(res));// use stdlib.h
        exit(EXIT_FAILURE); // Terminate the program with an error status

    }
    