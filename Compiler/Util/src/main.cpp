#include "./util.hpp"
var se_random_operation(var se_a, var se_b);
var se_fibonacci(var se_n);
var se_fibonacci_d(var se_n);
var se_iter_example();
var se_map_ex();
var se_default_ex(var se_a);
var se_set_ex();
var se_tuple_ex();
var se_slices_ex();
var se_string_ex();
var se_random_operation(var se_a, var se_b){
    var se_c;
    se_c = se_a + se_b;
    return se_c + se_a * se_b + var(2.6548);
    return var();
}
var se_fibonacci(var se_n){
    if((se_n == var(1) || se_n == var(2))) {
        return var(1);
    }
    else if(se_n == var(0)) {
        return se_n / var(0);
    }
    else{
        return se_fibonacci(se_n - var(1)) + se_fibonacci(se_n - var(2));
    }
    return var();
}
var se_fibonacci_d(var se_n){
    var se_n_1;
    var se_n_2;
    var se_new;
    se_n_1 = var(1);
    se_n_2 = var(1);
    while(se_n_1 < se_n){
        se_new = se_n_1 + se_n_2;
        se_n_2 = se_n_1;
        se_n_1 = se_new;
    }
    return se_n_1;
    return var();
}
var se_iter_example(){
    var se_l;
    var se_it;
    se_l = Builtin::inlineList({var(1), var(2.5), var(3), var("hola"), var(5), var("mundo")});
    se_it = Builtin::iter({se_l});
    for (auto se_i : (var) se_l){
        std::cout << Builtin::next({se_it}) << std::endl;
    }
    return true;
    return var();
}
var se_map_ex(){
    var se_d;
    se_d = Builtin::inlineDict({
        Pair(var("hola"), var("mundo")),
        Pair(var(1), Builtin::inlineList({var(1), var(2), var(3), var(4), var(5)})),
        Pair(var("dict"), Builtin::inlineDict({
            Pair(var("adios"), var(":D"))
        }))
    });
    for (auto se_k : (var) se_d->Call("keys", {})){
        std::cout << var(se_d->Call("slice", {se_k})) << std::endl;
    }
    return var("hola") + var("mundo");
    return var();
}
var se_default_ex(var se_a = var("hola")){
    return se_a;
    return var();
}
var se_set_ex(){
    var se_a;
    se_a = Builtin::inlineSet({var(1), var(2), var("hola"), var(4), var(5)});
    return var(se_a->Call("has", {var(2)}));
    return var();
}
var se_tuple_ex(){
    var se_a;
    var se_b;
    se_a = Builtin::inlineTuple({var(5), var(6), var("joseph")});
    se_b = Builtin::inlineTuple({var(1), var(2), var("valverde")});
    return se_a + se_b;
    return var();
}
var se_slices_ex(){
    var se_l;
    var se_k;
    se_l = Builtin::inlineList({var(1), var(2), var(3), var(4), var(5), var(6), var(7), var(7), var(8), var(9)});
    std::cout << var(se_l->Call("slice", {var(-2)})) << std::endl;
    std::cout << var(se_l->Call("slice", {var(1), var(-2)})) << std::endl;
    se_k = var(se_l->Call("slice", {var(1), var(2)})) + var(se_l->Call("slice", {var(-3), var(-4)}));
    return se_k;
    return var();
}
var se_string_ex(){
    std::cout << var(var("profe")->Call("slice", {var(2), var(4)})) << std::endl;
    std::cout << var(var("profe")->Call("slice", {var(2), var(4)})) + var(var("profe")->Call("slice", {var(0), var(2)})) + var(var("profe")->Call("slice", {var(-1)})) << std::endl;
    std::cout << se_random_operation(var(5), var(6)) << std::endl;
    return var();
}
int main(){
    std::cout << se_fibonacci(var(4)) << std::endl;
    std::cout << se_iter_example() << std::endl;
    std::cout << se_map_ex() << std::endl;
    std::cout << se_default_ex() << std::endl;
    std::cout << se_tuple_ex() << std::endl;
    std::cout << se_set_ex() << std::endl;
    std::cout << se_slices_ex() << std::endl;
    se_string_ex();
    return 0;
}