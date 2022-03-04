# Running and testing

To compile and test, from the parent directory:
```
R CMD INSTALL AwsSdkCrt
```
(Note: no need to do an R CMD build first)

Using:
```
library(AwsSdkCrt)

rcpp_hello_world()
```

### Adding a new file/function
* Add the new src/<name>.cpp file
It must include:
```
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double rcpp_sum(NumericVector v)
```

* Use Rcpp::compileAttributes() to update.
From this directory, start R:
```
library(Rcpp)
Rcpp::compileAttributes()
```
This will detect new functions/signatures and add them to RcppExports.cpp and RcppExpots.R.


# Building the CRT libs

https://docs.aws.amazon.com/sdkref/latest/guide/common-runtime.html

Install instructions in: https://github.com/awslabs/aws-c-cal
<install-path> is:  
/Users/alexwoo/R/crt_install

From the top level crt_build dir:

```

cmake -S aws-c-common -B aws-c-common/build -DCMAKE_INSTALL_PREFIX=/Users/alexwoo/R/crt_install
cmake --build aws-c-common/build --target install

cmake -S aws-c-cal -B aws-c-cal/build -DCMAKE_INSTALL_PREFIX=/Users/alexwoo/R/crt_install -DCMAKE_PREFIX_PATH=/Users/alexwoo/R/crt_install
cmake --build aws-c-cal/build --target install

cmake -S aws-c-compression -B aws-c-compression/build -DCMAKE_INSTALL_PREFIX=/Users/alexwoo/R/crt_install -DCMAKE_PREFIX_PATH=/Users/alexwoo/R/crt_install
cmake --build aws-c-compression/build --target install

cmake -S aws-checksums -B aws-checksums/build -DCMAKE_INSTALL_PREFIX=/Users/alexwoo/R/crt_install -DCMAKE_PREFIX_PATH=/Users/alexwoo/R/crt_install
cmake --build aws-checksums/build --target install

```

### Building aws-crt-ffi
Need to update api.h so that it exports useable symbols:
Add: `#include <aws/common/macros.h>`
And then wrap with `AWS_EXTERN_C_BEGIN` and `AWS_EXTERN_C_END`
Then replace the API exports with: `AWS_COMMON_API`

Need to also add: `#include <stdbool.h>`

Note, need to do a recusrive init, w/ all the submodules.
We build with shared libs off.
------------
```
cmake -S aws-crt-ffi -B aws-crt-ffi/build -DCMAKE_INSTALL_PREFIX=/Users/alexwoo/R/ffi/crt_install_2 -DBUILD_SHARED_LIBS=OFF
cmake --build aws-crt-ffi/build --target install

```

## Rcpp references
* https://teuder.github.io/rcpp4everyone_en/070_data_types.html
* https://dirk.eddelbuettel.com/code/rcpp/html/classRcpp_1_1String.html


## Types and conversions

### Strings
```
uint32_t rcpp_aws_checksums_crc32(StringVector input){

   std::string temp = Rcpp::as<std::string>(input[0]);
   const char* tmp_c_str = temp.c_str();
   return(aws_checksums_crc32((const uint8_t*)tmp_c_str, temp.size(), 0));
}
```


## testing
R CMD INSTALL AwsSdkCrt

```
library(AwsSdkCrt)

aws_crc32("test", 0)


```

Testing new code:
```
library(AwsSdkCrt)

c <- new(CredentialsOptions, 'akid', 'secret')
c$getAccessKeyId()
c$getSecretKey()

c <- NA
gc() # destructor should get called.

### Benchmarking
source("AwsSdkCrt/benchmark.R")

```
expr    min     lq     mean  median      uq    max neval
digest 18.142 20.039 22.51841 22.0545 23.6805 55.714  1000
 crt  6.904  8.289 10.04969  9.7265 11.3945 33.778  1000
 ```
