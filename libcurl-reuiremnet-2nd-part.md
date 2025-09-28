*****CURLOPT_POSTFIELDS - data to POST to server ****

`````
CURLcode curl_easy_setopt(CURL *handle, CURLOPT_POSTFIELDS, char postdata);

char *postdata ="Hello world" or postdata="name=John+Doe&email=john%40gmail.com&phone=1234567890&city=New+York"

 // set an endpoint uisng   CURLOPT_URL .. where data to be sent


    /* pass in a pointer to the data - libcurl does not copy */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Postdata);
 
    curl_easy_perform(curl);




Key-value pairs separated by &
Keys and values separated by =
Spaces become + or %20
Special characters are URL-encoded (like @ becomes %40)

`````

CURLOPT_POSTFIELDS  is a normally of content-type  application/x-www-form-urlencoded kind (i.e key vaule or raw data)

However the content type can be chnage with CURLOPT_HTTPHEADER to json or any other ...

curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list); 

Talk about it below just go through example and contwnt below

Example :

Headers → Metadata(information ) about request, type of request , sizeof it , where it's been sent.

Form → User-submitted data (because you used x-www-form-urlencoded).

Args → Query parameters (?id=1) or(?name=jhon)
Files → Uploaded files.

 {
  "args": {}, 
  "data": "", 
  "files": {}, 
  "form": {
    "email": "jhony@gmail.com", 
    "id": "21", 
    "name": "jhony sins", 
    "profession": "Actor"
  }, 
  "headers": {
    "Accept": "*/*", // accept any type of content json or aplicatipn
    "Content-Length": "62", 
    "Content-Type": "application/x-www-form-urlencoded", // actuall type of contetn recived 
    "Host": "httpbin.org", // who is reciving the content
    "X-Amzn-Trace-Id": "Root=1-68d8e951-60657123104c024656e2eb35"
  }, 


***** IF Json data is to be send then change the content-type with help of   CURLOPT_HTTPHEADER *******

1st we'll deifne the url endpoint 

2nd we'll define the custom header like host , content-type , Accept etc ...

3rd we'll pass the JSon pointer to  CURLOPT_POSTFIELDS to send the data

4th  we'll perform the reuest 


We can add new header like host , aceept ,content type etc .. as a linkd-list using 
  curl_slist_append to create the list and curl_slist_free_all to free it again after use.

 The Connection: header in HTTP/1.1 cannot be overridden. You can provide values for it, but should a request require specific ones,
 it can't be chnaged 


// defining a new linkidlist for custom header using 

curl_slist_append()


The existing list should be passed as the first argument and the new list is returned from this function

Pass in NULL in the list argument to create a new list

```
  struct curl_slist *list = NULL; // creating a new list

  list = curl_slist_append(list, "Content-Type: application/json");

  
  list = curl_slist_append(list, "Accept: */*");  

  +----------------------------+     +---------------------+   +----------------+
| "Content-Type: application/json" |-->| "Accept: */*" |-->|      NULL   |
+----------------------------+     +---------------------+     +----------------+

// this is our custom header list 


pass this list through  
 curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list); // this willl send the custom header 


curl_slist_free_all(list); // free the linkd-list meemory  when done


  THus we created a new custom header linkdlist with content-type json/appication and saying server to accept type any (i.e */* )
```
