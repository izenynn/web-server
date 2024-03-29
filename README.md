# web-server

## Info

This project aims to create a basic HTTP server following the HTTP/1.1 standard in C++98.

- Status: finished
- Result: 125%
- Observations: null

## Config file

Configuration is similar to nginx, you can check some examples on the folder `conf/`.

A brief explanetion of the supported blocks:

- `server`: same as nginx server block.
- `location`: same as nginx location block.

A brief explanetion of server block only directives:

- `listen`: `ip:port`: ip:port to listen on, you specify only the ip, or only the port, or none; defualt values are `0.0.0.0:80`.
- `server_name`: list of names that will be used to determine the server if more than one server is listening on the same ip:port, useful for virtual hosting.

A brief explanetion of server/location blocks directives:

- `root`: root folder for the server files, defualt is `/var/www/`.
- `alias`: only for location block (doesnt make sense in server use root directive there); must end on `"/"` because is a directory; like the ngix directive is used to make a location alias to other directory, if present root directive will be ignored, more info [here](https://stackoverflow.com/questions/10631933/nginx-static-file-serving-confusion-with-root-alias).
- `index`: index file that should be returnde when client makes a GET request to the directory
- `autoindex`: `on/off`: autoindex page when requesting a directory; defualt is `off`.
- `error_page`: `404 example.html`: specify a custom html file for an error code.
- `limit_except`: `GET POST PUT DELETE`: specify allowed methods on that server/location block.
- `client_max_body_size`: max size for a client request body.
- `upload_store`: path where uploaded files with POST/PUT will be saved, this path is relative to `root` location, if not specified files will be uploaded to location directory or `root` in case not inside `location` block.
- `return`: only for location block; define HTTP redirections (`301`, `302`, `303` or `307`), to a full url (`http://[...]`) or a local url (`/home/`), you can also define any custom response with `(1XX | 2XX | 4XX | 5XX) ["text"]` and the text that will appear on the body (but text is optional, default text for the status code will appear if not specified).
- `cgi`: p.e.: `cgi .php /usr/bin/php-cgi`, specify a CGI binary for a file extension.

NOTE: all directives are based on nginx ones, so you can search on nginx wiki for more info, but keep in mind my implementation is not 100% as nginx, mostly because limitations on available directives force me to change a little the behaviour of the existing ones in order to offer a better experience.

## How to use

### Clone
```sh
git clone --recurse-submodules https://github.com/izenynn/web-server.git
```

### Compile and execute
```sh
make
```
```sh
./webserv [CONF FILE]
```

NOTE: if not conf file specified it will search for one on the defualt path: `/etc/webserv/webserv.conf`.

##

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/powered-by-coffee.svg)](https://forthebadge.com)
