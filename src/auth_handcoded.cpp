
#include <Rcpp.h>

#include <aws/auth/credentials.h>
#include <aws/io/event_loop.h>
#include <aws/io/channel_bootstrap.h>
#include <aws/common/string.h>
#include <aws/common/byte_buf.h>



#include "wrapper.h"
using namespace Rcpp;


Wrapper::Wrapper(int value) {
  this->value = value;
}

Wrapper::~Wrapper() {
  printf("DESTRUCTOR!!!!!!!!\n");
}


// Can be called as: test_callback(1:10, function(x) x*x)
// [[Rcpp::export]]
NumericVector test_callback(NumericVector numvec, Function f){
    return f( numvec ) ; ;
}

// [[Rcpp::export]]
XPtr< Wrapper > rcpp_make_internal() {
  Wrapper* w = new Wrapper(1234);
  printf("BUILT A WRAPPER, its value is: %d\n", w->value);
  Rcpp::XPtr< Wrapper > p(w, true) ;
  return p;
}

// [[Rcpp::export]]
int rcpp_use_internal(XPtr< Wrapper > p) {
  printf("using A WRAPPER, its value is: %d\n", p->value);

  return p->value;
}

static void s_on_shutdown_complete(void *user_data) {
  (void)user_data;
  printf("Shutdown complete called.\n");
}

static void s_get_credentials_callback(struct aws_credentials *credentials, int error_code, void *user_data) {
  printf("Retrieved credentials from default credential provider chain.\nAccess key: %s\n", aws_credentials_get_access_key_id(credentials).ptr);
}

// [[Rcpp::export]]
uint32_t aws_credentials_from_chain(){
  // Currently blows up - probably needs a correctly initialized client bootstrap
  // that requires an event loop group and shutdown callbacks, ect....

  struct aws_allocator *allocator = aws_default_allocator();
  struct aws_event_loop_group *el_group = aws_event_loop_group_new_default(allocator, 1, NULL);

   struct aws_host_resolver_default_options resolver_options = {
       .el_group = el_group,
       .max_entries = 8,
   };

   struct aws_host_resolver *resolver = aws_host_resolver_new_default(allocator, &resolver_options);

   struct aws_client_bootstrap_options bootstrap_options = {
       .event_loop_group = el_group,
       .host_resolver = resolver,
   };
   struct aws_client_bootstrap *bootstrap = aws_client_bootstrap_new(allocator, &bootstrap_options);

   struct aws_credentials_provider_chain_default_options options = {
         .bootstrap = bootstrap,
         .tls_ctx = NULL,
         .shutdown_options =
             {
                 .shutdown_callback = s_on_shutdown_complete,
                 .shutdown_user_data = NULL,
             },
     };

  aws_credentials_provider* creds = aws_credentials_provider_new_chain_default(aws_default_allocator(), &options);

  aws_credentials *credentials;
  int ret = aws_credentials_provider_get_credentials(creds, s_get_credentials_callback, NULL);


  aws_client_bootstrap_release(bootstrap);
  aws_host_resolver_release(resolver);
  aws_event_loop_group_release(el_group);


  return 0;
}
