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

Note, need to do a recusrive init, w/ all the submodules.
------------
```
cmake -S aws-crt-ffi -B aws-crt-ffi/build -DCMAKE_INSTALL_PREFIX=/Users/alexwoo/R/ffi/crt_install
cmake --build aws-crt-ffi/build --target install

```

## Rcpp references
* https://teuder.github.io/rcpp4everyone_en/070_data_types.html
* https://dirk.eddelbuettel.com/code/rcpp/html/classRcpp_1_1String.html


## Types and concersions

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

rcpp_aws_checksums_crc32("test", 0)

rcpp_handcoded_aws_checksums_crc32("test", 0) # should give 3632233996

```