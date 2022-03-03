#include <api.h>
#include <Rcpp.h>

// [[Rcpp::export]]
void rcpp_aws_crt_init(){
  printf("init!\n");
   aws_crt_init();
   return;
}
