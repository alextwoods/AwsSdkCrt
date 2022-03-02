
#include <Rcpp.h>
#include <aws/common/math.h>
#include <aws/common/clock.h>
#include <aws/common/date_time.h>

using namespace Rcpp;

// [[Rcpp::export]]
List rcpp_hello_world() {

    CharacterVector x = CharacterVector::create( "foo", "bar" )  ;
    NumericVector y   = NumericVector::create( 0.0, 1.0 ) ;
    List z            = List::create( x, y ) ;

    return z ;
}

// [[Rcpp::export]]
List warble_garble() {

    uint64_t testValue = aws_add_u64_saturating(1,3);
    uint64_t time;
    aws_sys_clock_get_ticks(&time);



    aws_date_time dt;
    aws_date_time_init_now(&dt);


    CharacterVector x = CharacterVector::create( "warble", "garble2" )  ;
    NumericVector y   = NumericVector::create( time, testValue) ;
    List z            = List::create( x, y ) ;

    return z ;
}
