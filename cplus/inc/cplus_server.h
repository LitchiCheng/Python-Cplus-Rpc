#ifndef _CPLUS_SERVER_H_
#define _CPLUS_SERVER_H_

#include <thread>
#include <memory>
#include <string>
#include "zmq.hpp"

#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <utility>
#include <tuple>
#include <iostream>

#include "json.hpp"
using json = nlohmann::json;
using namespace std;

template<typename T>
struct type_xx{	typedef T type; };

template<>
struct type_xx<void>{ typedef int8_t type; };

template<typename Function, typename Tuple, std::size_t... Index>
decltype(auto) invoke_impl(Function&& func, Tuple&& t, std::index_sequence<Index...>){
	return func(std::get<Index>(std::forward<Tuple>(t))...);
}

template<typename Function, typename Tuple>
decltype(auto) invoke(Function&& func, Tuple&& t){
	constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
	return invoke_impl(std::forward<Function>(func), std::forward<Tuple>(t), std::make_index_sequence<size>{});
}

template<typename R, typename F, typename ArgsTuple>
typename std::enable_if<std::is_same<R, void>::value, typename type_xx<R>::type >::type
call_helper(F f, ArgsTuple args) {
	invoke(f, args);
	return 0;
}

template<typename R, typename F, typename ArgsTuple>
typename std::enable_if<!std::is_same<R, void>::value, typename type_xx<R>::type >::type
call_helper(F f, ArgsTuple args) {
	return invoke(f, args);
}

class CplusServer
{
public:
    CplusServer(std::string addr);
    ~CplusServer();
    void run();

    template<typename F>
	void bind(std::string name, F func);

	template<typename F, typename S>
	void bind(std::string name, F func, S* s);
    
private:
    json* call(std::string name, json& method_args);

	template<typename R, typename... Params>
	void callproxy_(R(*func)(Params...), json* ret, json& rec) {
		callproxy_(std::function<R(Params...)>(func), ret, rec);
	}

	template<typename R, typename C, typename S, typename... Params>
	void callproxy_(R(C::* func)(Params...), S* s, json* ret, json& rec) {
		using args_type = std::tuple<typename std::decay<Params>::type...>;

		constexpr auto N = std::tuple_size<typename std::decay<args_type>::type>::value;
		args_type args = get_tuple < args_type >(std::make_index_sequence<N>{}, rec);

		auto ff = [=](Params... ps)->R {
			return (s->*func)(ps...);
		};
		typename type_xx<R>::type r = call_helper<R>(ff, args);
		cout << "r is " << r << endl; 
        (*ret)["res"] = r;
	}

	template<typename R, typename... Params>
	void callproxy_(std::function<R(Params... ps)> func, json* ret, json& rec) {
		using args_type = std::tuple<typename std::decay<Params>::type...>;
		constexpr auto N = std::tuple_size<typename std::decay<args_type>::type>::value;
		args_type args = get_tuple < args_type >(std::make_index_sequence<N>{}, rec);
		typename type_xx<R>::type r = call_helper<R>(func, args);
		(*ret)["res"] = r;
	}

	template<typename Tuple, std::size_t Id>
	void getv(json& ds, Tuple& t) {
		std::get<Id>(t) = ds["method_args"][Id];
	}

	template<typename Tuple, std::size_t... I>
	Tuple get_tuple(std::index_sequence<I...>, json& rec) {
		Tuple t;
		std::initializer_list<int>{((getv<Tuple, I>(rec, t)), 0)...};
		return t;
	}

	template<typename F>
	void callproxy(F fun, json* ret, json& rec);

	template<typename F, typename S>
	void callproxy(F fun, S* s, json* ret, json& rec);

private:
    zmq::context_t _ctx{(1)};
    zmq::socket_t _socket;
    std::string _addr{"tcp://*:5555"};
    std::map<std::string, std::function<void(json*, json&)>> _handlers;
};

#endif //_CPLUS_SERVER_H_
