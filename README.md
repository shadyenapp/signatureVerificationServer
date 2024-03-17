# Signature Verification Server in C

Base socket/file transfer server provided by [nikhilroxtomar](https://github.com/nikhilroxtomar/File-Transfer-using-TCP-Socket-in-C/blob/master/server.c). Modified to meet needs for a project, the verification of x.509 signatures and then running a script. 

## Setup
Before running the client or server a private and public keypair is needed. In my tests, I used an RSA and sha256 keypair to mimic an x.509 certificate.
To generate this keypair the following commands must be ran:
```Shell
openssl genrsa -out private.pem 4096
openssl rsa -in private.pem -pubout -out public.pem
```

## Client
Before running, signature and script file must be compiled into a tarball using: 
```Shell
tar -czvf files.tar.gz [signature] [script.sh]
```
The signature can be made using openssl and a public/private keypair:
```Shell
openssl dgst -sha256 -sign [private key] -out [signature] [file]
```
The Client then takes the compressed tarball and sends it to the server. Note that the script must be named _script.sh_ and the signature file must be named _sig_ to match the code. 
## Server
The server takes the tarball recieved from the client, unzips it into _sig_ and _script.sh_. If the script is verified by the said signature, the server will run the script and exit.  

## Make and Autocompile
The make file compiles both server and client using:
```Shell
make all
```
## Example outputs:
The outputs and test cases done below are using a script.sh file that is one line:
```Shell
echo "test"
```
**Server Success with verified signature**
<br>
![Server Sucess](https://github.com/shadyenapp/signatureVerificationServer/blob/main/TestCaseImages/ServerOutputSuccess.png)
<br>
**Server Failure with unmatching signature**
<br>
![Server Failure](https://github.com/shadyenapp/signatureVerificationServer/blob/main/TestCaseImages/ServerOutputFailure.png)
<br>
**Client Success in sending file**
<br>
![Client Success](https://github.com/shadyenapp/signatureVerificationServer/blob/main/TestCaseImages/ClientOutputSuccess.png)
