#include <iostream>
#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <vector>
#include "rules.h"
#include "game_engine.h"

using namespace std;

TEST(dummy, test) {
    EXPECT_EQ(1, 1);
}

TEST(dummy, asio) {
    bool check = false;
    boost::asio::io_context ioc;
    boost::asio::steady_timer timer(ioc, boost::asio::chrono::seconds(2));
    timer.async_wait(std::bind([&check](){
        check = true;
        cout << "!!!done!!!" << endl;
    }));

    auto a = std::thread([&ioc]() {
        ioc.run();
    });

    a.join();
    EXPECT_TRUE(check);
}

void print( boost::asio::steady_timer& t, int& count){
    if (count < 5){
        (count)++;
        t.expires_at(t.expires_at() + boost::asio::chrono::seconds(2));

        //  The steady_timer::async_wait() function expects a handler function
        //  (or function object) with the signature void(const boost::system::error_code&).
        //  Binding the additional parameters converts your print function into a function
        //  object that matches the signature correctly.
        t.async_wait(boost::bind(print, ref(t), ref(count)));
    }
}

TEST(dummy, free_functions) {
    int count = 0;
    boost::asio::io_context ioc;
    boost::asio::steady_timer timer(ioc);
    timer.expires_after(boost::asio::chrono::seconds(2));

    timer.async_wait(boost::bind(print, ref(timer), ref(count)));
    auto tr = thread([&ioc](){
        ioc.run();
    });
    tr.join();
    EXPECT_EQ(5, count);
}

class Printer {
public:
    Printer(boost::asio::io_context& ioc):timer_(ioc, boost::asio::chrono::seconds(2)), count_(0) {
        timer_.async_wait(boost::bind(&Printer::print, this));
    }
    void print() {
        cout << " done " << endl;
        if (count_ <5) {
            count_ ++;
            timer_.expires_at(timer_.expires_at() + boost::asio::chrono::seconds(2));
            // The boost::bind() function works just as well with class
            // member functions as with free functions. Since all
            // non-static class member functions have an implicit
            // "this" parameter, we need to bind "this" to the function.
            timer_.async_wait(boost::bind(&Printer::print, this));
        }
    }
    int getCount() {
        return count_;
    }
private:
    boost::asio::steady_timer timer_;
    int count_;
};

TEST(dummy, member_functions) {
    boost::asio::io_context ioc;
    auto p = Printer(ioc);
    auto t = thread ([&ioc]() {
        ioc.run();
    });
    t.join();
    EXPECT_EQ(5, p.getCount());
}

class Publisher {
public:
    Publisher(){
    }
    boost::signals2::connection subscribe(boost::signals2::slot<void(), boost::function<void()>> a){
         return subscription_.connect(a);
    }
    void publish(){
        subscription_();
    }
private:
    boost::signals2::signal<void()> subscription_;
};

class Subscriber {
public:
    int& counts_;
    Subscriber(int& count): counts_(count){
        cout<<"const"<<endl;
    }
    ~Subscriber(){
        cout<<"destr"<<endl;
    }

    void test(){
        cout<< "blaaaa " << endl;
        counts_++;
    }
};

TEST(dummy, signals) {
    boost::signals2::signal<void()> subscription_;
    EXPECT_EQ(0, subscription_.num_slots());
    {
        int num = 0;
        auto da = std::make_shared<Subscriber>(num);
        boost::signals2::slot<void(), boost::function<void()>> c = boost::signals2::signal<void()>::slot_type(&Subscriber::test,da.get());
        boost::signals2::slot<void(), boost::function<void()>> d = c.track_foreign(da);
        auto b = subscription_.connect(d);
        subscription_();
        EXPECT_EQ(1, da->counts_);
        EXPECT_EQ(1, subscription_.num_slots());
        EXPECT_EQ(1, da.use_count());
    }
    subscription_();
    EXPECT_EQ(0, subscription_.num_slots());
}

TEST(dummy, signals2) {
    int count = 0;
    auto pub = Publisher();
    {
        auto sub = std::make_shared<Subscriber>(count);
        boost::signals2::slot<void(), boost::function<void()>> c = boost::signals2::signal<void()>::slot_type(&Subscriber::test, sub.get());
        boost::signals2::slot<void(), boost::function<void()>> d = c.track_foreign(sub);
        pub.subscribe(d);
        pub.publish();
        EXPECT_EQ(1, count);
        EXPECT_EQ(1, sub.use_count());
    }
    pub.publish();
    EXPECT_EQ(1, count);
}

class timer {
public:
    timer (boost::asio::io_context& ioc, boost::asio::chrono::seconds sec, int& timer_count):t_(ioc, sec), timer_count_(timer_count){
        t_.async_wait(std::bind(&timer::timer_handle, this));
    }
    void timer_handle(){
        cout<<"handle"<<endl;
        timer_count_++;
        if (timer_count_ < 5){
            t_.expires_at(t_.expires_at() + boost::asio::chrono::seconds(1));
            t_.async_wait(std::bind(&timer::timer_handle, this));
        }
    }
private:
    boost::asio::steady_timer t_;
    int& timer_count_;
};

class timer_wrapper: public timer {
public:
    timer_wrapper(boost::asio::io_context& ioc, boost::asio::chrono::seconds sec, int& timer_count):timer(ioc, sec, timer_count){

    }
};

TEST(dummy, timer_inherited) {
    int timer_count = 0;
    boost::asio::io_context ioc;
    auto t = timer_wrapper(ioc, boost::asio::chrono::seconds(1), timer_count);

    auto thr = thread([&ioc]{
        ioc.run();
    });
    thr.join();
    EXPECT_EQ(5, timer_count);
}

class Cell2 {
public:
    Cell2(int num):num_(num){}
    int Get() const {
        return num_;
    }
    void Set(int num) {
        num_ = num;
    }
private:
    int num_;
};

TEST(dummy, matrix) {
//    9 0 0 0 -> rows = x
//    0 0 0 0
//    0 0 0 9
    int rows = 4;
    int columns = 3;
    std::vector<std::vector<Cell2>> matrix;

    std::generate_n(std::back_inserter(matrix), rows, [&columns]() {
        std::vector<Cell2> matrix_columns;
        std::generate_n(std::back_inserter(matrix_columns), columns, []() {
            return Cell2(0);
        });
        return matrix_columns;
    });
    //     x  y
    matrix[0][0].Set(9);
    matrix[3][2].Set(9);
}

TEST(dummy, shared_matrix) {
    //    9 0 0 0 -> rows = x
//    0 0 0 0
//    0 0 0 9
    int rows = 4;
    int columns = 3;
    std::vector<std::vector<Cell2>> matrix;

    std::generate_n(std::back_inserter(matrix), rows, [&columns]() {
        std::vector<Cell2> matrix_columns;
        std::generate_n(std::back_inserter(matrix_columns), columns, []() {
            return Cell2(0);
        });
        return matrix_columns;
    });
    //     x  y
    matrix[0][0].Set(9);
    matrix[3][2].Set(9);

    std::shared_ptr<std::vector<std::vector<Cell2>>> shared_matrix = std::make_shared<std::vector<std::vector<Cell2>>> (std::move(matrix)); // move!
    (*shared_matrix)[1][1].Set(9);
    EXPECT_EQ(9, (*shared_matrix)[1][1].Get()); // dereference tje shared pointer
}


TEST(dummy, make_share_of_struct) {
    struct State {
        int a;
        int b;
        int c;
    };

    auto shared_state = std::make_shared<State>(State{1,2,3});
    EXPECT_EQ(1, shared_state->a);
    EXPECT_EQ(2, shared_state->b);
    EXPECT_EQ(3, shared_state->c);
}

TEST(dummy, async_test) {
    bool check = false;
    std::cout<<" >>>>> " <<std::endl;
    {
        auto ioc_future = std::unique_ptr<int, std::function<void(int*)>> (
                new int(1), [&check](int* a){
                    check = true;
                    std::cout<<" custom destructor: " << *a <<std::endl;
                    delete(a);
                });
    }
    std::cout<<" <<<<< " <<std::endl;
    EXPECT_TRUE(check);
}

#include <fstream>
#include <sstream>

std::string Read(const std::string& filename){
    std::fstream file;
    file.open(filename.c_str(), std::fstream::in);
    if (!file.is_open()) {
        cout<< "cannot read file" <<endl;
    }
    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();

    return contents.str();
}

#include <unistd.h>
#include <algorithm>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/lexical_cast.hpp>
TEST(dummy, PID) {
    pid_t pid = getpid();
    cout << pid << endl;
    std::string pid_status_file = "/proc/" + std::to_string(pid) + "/status";
    cout << pid_status_file << endl;

    std::string status = Read("/Users/user/CLionProjects/game_of_life/project/game_of_life/test/status"s);

    const std::string kVmDataTag = "VmData:";
    const int l = kVmDataTag.length();
    const std::string kKBTag = "kB";
    std::size_t begin = status.find(kVmDataTag);
    EXPECT_NE(begin, std::string::npos);
    cout << "begin: " << begin << endl;

    std::size_t end = status.substr(begin + l).find(kKBTag);
    EXPECT_NE(end, std::string::npos);
    cout << "end: " << end << endl;

    std::string vmData = status.substr(begin + l, end);
    boost::algorithm::trim_all(vmData);
    EXPECT_EQ("54060", vmData);

    try {
        int vmData_value = boost::lexical_cast<int>(vmData);
        EXPECT_EQ(54060, vmData_value);
    }
    catch (const boost::bad_lexical_cast &e) {
        std::cout << e.what() << std::endl;
    }
}

class NonCopyableBase {
protected:
    NonCopyableBase() = default;
    virtual ~NonCopyableBase() = default;

    NonCopyableBase(const NonCopyableBase&) = delete;
    NonCopyableBase(NonCopyableBase&&) = delete;

    NonCopyableBase& operator=(const NonCopyableBase&) = delete;
    NonCopyableBase& operator=(NonCopyableBase&&) = delete;
};

class animal : public NonCopyableBase {
public:
    virtual std::string walk() = 0;
    virtual std::string talk() = 0;
};

class cow : public animal {
public:
    std::string walk(){
        return "step by step";
    }
    std::string talk(){
        return "boooooooo";
    }
};

class monkey : public animal {
public:
    std::string walk(){
        return "swing swing swing";
    }
    std::string talk(){
        return "hoehoehoehoe";
    }
};

std::unique_ptr<animal> animalFactory(std::string a){
    if (a == "monkey") {
        return std::make_unique<monkey>();
    }
    return std::make_unique<cow>();
}

TEST(dummy, inherit) {
    animal* b = new cow();                      // child -> parent
    EXPECT_EQ("boooooooo", b->talk());
    EXPECT_EQ("step by step", b->walk());

    cow* c = dynamic_cast<cow*>(b);             // parent -> child
    EXPECT_EQ("boooooooo", c->talk());
    EXPECT_EQ("step by step", c->walk());

    std::unique_ptr<animal> d = std::make_unique<cow>();                // child -> parent
    EXPECT_EQ("boooooooo", d->talk());
    EXPECT_EQ("step by step", d->walk());


    std::unique_ptr<animal> animal;

    animal = animalFactory("monkey");
    EXPECT_EQ("hoehoehoehoe", animal->talk());
    EXPECT_EQ("swing swing swing", animal->walk());

    animal = animalFactory("cow");
    EXPECT_EQ("boooooooo", animal->talk());
    EXPECT_EQ("step by step", animal->walk());
}

class asdf {
public:
    void qwer(int arg1, int arg2) {
        arg1_ = arg1;
        arg2_ = arg2;
    }
    int arg1_;
    int arg2_;
};

TEST(dummy, inherit2) {
    asdf c;
    c.qwer(1,2);
    EXPECT_EQ(1, c.arg1_);
    EXPECT_EQ(2, c.arg2_);
}




