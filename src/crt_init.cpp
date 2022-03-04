#include <api.h>
#include <Rcpp.h>

// [[Rcpp::export]]
void rcpp_aws_crt_init(){
   aws_crt_init();
   printf("CRT initialized!\n");
   return;
}
