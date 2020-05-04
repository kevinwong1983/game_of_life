//#include "boost/variant.hpp"
//#include <iostream>
//#include <gtest/gtest.h>
//#include <boost/asio.hpp>
//#include <time.h>
//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/system/system_error.hpp>
//#include <functional>
//#include <unordered_map>
//
//#include "mosquitto_mqtt_client.h"
//#include "mqtt_publish_options.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/types.h>
//
//class MqttTest : public testing::Test {
//public:
//    void SetUp(){
//        fire_up_mosquitto_broker();
//    }
//    void TearDown(){
//        shut_down_mosquitto_broker();
//        sleep(1); // give some time for port to be available again
//    }
//    void async_sleep(boost::asio::io_context &ioc, int seconds) {
//        std::promise<std::string> done;
//        auto t = boost::asio::steady_timer (ioc, boost::asio::chrono::seconds(seconds));
//        t.async_wait(std::bind([&done](){
//            done.set_value("done");
//        }));
//        std::cout << done.get_future().get() << std::endl;
//    }
//private:
//    void fire_up_mosquitto_broker() {
//        pid_t processId;
//        if ((processId = fork()) == 0) {
//            char app[] = "/usr/local/sbin/mosquitto";
//            char arg1[] = "-c";
//            char arg2[] = "/usr/local/etc/mosquitto/mosquitto.conf";
//            char * const argv[] = { app, arg1, arg2 , NULL };
//            std::cout<< "setup mosquitto" <<std::endl;
//            execv (app, argv);
//        }
//        sleep(1);
//    }
//    void shut_down_mosquitto_broker(){
//        system ("killall mosquitto");
//    }
//};
//
//TEST_F(MqttTest, mqtt_client_can_subscribe_and_publish) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{ "127.0.0.1", 1883,"hello", ioc};
//
//    // and mqtt client A subscribed to topic "hello/world".
//    int number_of_messages_received = 0;
//    mqttClient.Subscribe("hello/world", [&number_of_messages_received, &mqttClient](const WildcardValue& wc, const Message& m){
//        EXPECT_EQ(m, "HELLO WORLD");
//        EXPECT_TRUE(wc.empty());
//        number_of_messages_received ++;
//
//        mqttClient.Disconnect();
//    });
//
//    // when mqtt client A publishes to "hello/world".
//    mqttClient.Publish("hello/world", "HELLO WORLD");
//
//    // then published message received by subscriber.
//    ioc.run();
//    EXPECT_EQ(number_of_messages_received, 1);
//}
//
//// NOTE it is not possible to create the client after ioc run.
//TEST_F(MqttTest, mqtt_client_can_subscribe_and_publish_after_ioc_run) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{ "127.0.0.1", 1883, "hello",ioc};
//    // and ioc run
//    auto t = std::thread([&ioc](){
//        ioc.run();
//    });
//    sleep(1);
//
//    // when mqtt client A subscribes to "hello/world".
//    int number_of_messages_received = 0;
//    mqttClient.Subscribe("hello/world", [&number_of_messages_received, &mqttClient](const WildcardValue& wc, const Message& m){
//        EXPECT_EQ(m, "HELLO WORLD");
//        EXPECT_TRUE(wc.empty());
//        number_of_messages_received ++;
//
//        mqttClient.Disconnect();
//    });
//
//    // and mqtt client A publishes to "hello/world".
//    mqttClient.Publish("hello/world", "HELLO WORLD");
//
//    // then published message received by subscriber.
//    t.join();
//    EXPECT_EQ(number_of_messages_received, 1);
//}
//
//TEST_F(MqttTest, mqtt_client_can_have_multiple_subscriptions_on_same_topic) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{"127.0.0.1", 1883,"hello",  ioc};
//
//    int number_of_messages_received = 0;
//    auto subscribe_handler = [&number_of_messages_received, &mqttClient](const WildcardValue& wc, const Message& m){
//        EXPECT_EQ(m, "HELLO WORLD");
//        EXPECT_TRUE(wc.empty());
//        number_of_messages_received ++;
//        if (number_of_messages_received == 3){
//            mqttClient.Disconnect();
//        }
//    };
//    // and mqtt client A subscribed to "hello/world".
//    mqttClient.Subscribe("hello/world", subscribe_handler);
//    // and mqtt client A subscribed to "hello/world".
//    mqttClient.Subscribe("hello/world", subscribe_handler);
//    // and mqtt client A subscribed to "hello/world".
//    mqttClient.Subscribe("hello/world", subscribe_handler);
//
//    // when mqtt client A publishes to "hello/world".
//    mqttClient.Publish("hello/world", "HELLO WORLD");
//
//    // then published message received by subscriber.
//    ioc.run();
//    EXPECT_EQ(number_of_messages_received, 3);
//}
//
//TEST_F(MqttTest, mqtt_client_can_have_multiple_publications_on_same_topic) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{"127.0.0.1", 1883, "hello", ioc};
//
//    auto messages = std::vector <std::string>();
//    auto subscribe_handler = [&messages, &mqttClient](const WildcardValue& wc, const Message& m){
//        messages.emplace_back(m);
//        EXPECT_TRUE(wc.empty());
//        if (messages.size() == 3){
//            mqttClient.Disconnect();
//        }
//    };
//    // and mqtt client A subscribed to "hello/world".
//    mqttClient.Subscribe("hello/world", subscribe_handler);
//
//    // when mqtt client A publishes to "hello/world".
//    mqttClient.Publish("hello/world", "HELLO WORLD_1");
//    // and mqtt client A publishes to "hello/world" .
//    mqttClient.Publish("hello/world", "HELLO WORLD_2");
//    // and mqtt client A publishes to "hello/world".
//    mqttClient.Publish("hello/world", "HELLO WORLD_3");
//
//    // then all published messages received by subscriber.
//    ioc.run();
//    EXPECT_EQ(3, messages.size());
//    EXPECT_EQ(1, std::count_if(begin(messages), end(messages), [](std::string message){return message.compare("HELLO WORLD_1") == 0;}));
//    EXPECT_EQ(1, std::count_if(begin(messages), end(messages), [](std::string message){return message.compare("HELLO WORLD_2") == 0;}));
//    EXPECT_EQ(1, std::count_if(begin(messages), end(messages), [](std::string message){return message.compare("HELLO WORLD_3") == 0;}));
//}
//
//TEST_F(MqttTest, mqtt_client_A_can_subscribe_to_mqtt_publication_from_client_B) {
//    // given connected mqtt client A and B.
//    boost::asio::io_context ioc;
//    MqttClient mqttClientA{"127.0.0.1", 1883, "hello1", ioc};
//    MqttClient mqttClientB{"127.0.0.1", 1883, "hello2", ioc};
//
//    // and mqtt client A subscribed to "hello/world".
//    int number_of_messages_received = 0;
//    mqttClientA.Subscribe("hello/world", [&number_of_messages_received, &mqttClientA, &mqttClientB](const WildcardValue& wc, const Message& m){
//        EXPECT_EQ(m, "HELLO WORLD");
//        EXPECT_TRUE(wc.empty());
//        number_of_messages_received ++;
//
//        mqttClientA.Disconnect();
//        mqttClientB.Disconnect();
//    });
//
//    // when mqtt client B publishes to "hello/world".
//    mqttClientB.Publish("hello/world", "HELLO WORLD");
//
//    // then published message received by subscriber.
//    ioc.run();
//    EXPECT_EQ(number_of_messages_received, 1);
//}
//
//TEST_F(MqttTest, mqtt_client_can_subscribe_and_publish_with_single_level_wild_card) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{"127.0.0.1", 1883, "hello", ioc};
//
//    // and mqtt client A subscribed to topic with single level wildcard".
//    int number_of_messages_received = 0;
//    mqttClient.Subscribe("hello/+/world", [&number_of_messages_received, &mqttClient](const WildcardValue& wc, const Message& m){
//        EXPECT_EQ(m, "HELLO WORLD");
//        EXPECT_EQ(wc, "id1");
//        number_of_messages_received ++;
//
//        mqttClient.Disconnect();
//    });
//
//    // when mqtt client A publishes to "hello/id/world".
//    mqttClient.Publish("hello/id1/world", "HELLO WORLD");
//
//    // then published message received by subscriber.
//    ioc.run();
//    EXPECT_EQ(number_of_messages_received, 1);
//}
//
//// this does not work, while mqtt does support this.
//TEST_F(MqttTest, DISABLED_mqtt_client_can_subscribe_and_publish_with_multiple_single_level_wild_cards) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{"127.0.0.1", 1883,"hello",  ioc};
//
//    // and mqtt client A subscribed to topic with multiple single level wildcards".
//    int number_of_messages_received = 0;
//    mqttClient.Subscribe("hello/+/+/world", [&number_of_messages_received, &mqttClient](const WildcardValue& wc, const Message& m){
//        EXPECT_EQ(m, "HELLO WORLD");
//        EXPECT_EQ(wc, "id");
//        number_of_messages_received ++;
//
//        mqttClient.Disconnect();
//    });
//
//    // when mqtt client A publishes to "hello/id/world".
//    mqttClient.Publish("hello/id1/id2/world", "HELLO WORLD");
//
//    // then published message received by subscriber.
//    ioc.run();
//    EXPECT_EQ(number_of_messages_received, 1);
//}
//
//TEST_F(MqttTest, mqtt_client_can_subscribe_and_publish_with_multi_level_wild_card) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{"127.0.0.1", 1883,"hello",  ioc};
//
//    // and mqtt client A subscribed to topic with multi level wildcard".
//    int number_of_messages_received = 0;
//
//    // note: muti-level subscription using root/#/leaf results
//    // in "invalid subscription string from 127.0.0.1, disconnecting." error
//    // and disconnection of client.
//    mqttClient.Subscribe("hello/#", [&number_of_messages_received, &mqttClient](const WildcardValue& wc, const Message& m){
//        EXPECT_EQ(m, "HELLO WORLD");
//        EXPECT_EQ(wc, "id/world");
//        number_of_messages_received ++;
//
//        mqttClient.Disconnect();
//    });
//
//    // when mqtt client A publishes to "hello/id/world".
//    mqttClient.Publish("hello/id/world", "HELLO WORLD");
//
//    // then published message received by subscriber.
//    ioc.run();
//    EXPECT_EQ(number_of_messages_received, 1);
//}
//
//TEST_F(MqttTest, non_retained_published_messages_are_not_received_after_subscription) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{"127.0.0.1", 1883, "hello", ioc};
//    auto timer = boost::asio::steady_timer(ioc, boost::asio::chrono::seconds(3));
//    timer.async_wait(std::bind([&mqttClient](){
//        mqttClient.Disconnect();
//    }));
//
//    // and ioc run
//    auto t = std::thread([&ioc](){
//        ioc.run();
//    });
//    sleep(1);
//
//    // and mqtt client A publishes to "hello/world" as a non-retained message.
//    auto option = MqttPublishOptions();
//    option.Retain = false;
//    mqttClient.Publish("hello/world", "HELLO WORLD", option);
//    sleep(1);
//
//    // when mqtt client A subscribed to topic "hello/world".
//    int number_of_messages_received = 0;
//    mqttClient.Subscribe("hello/world", [&number_of_messages_received, &mqttClient](const WildcardValue& wc, const Message& m){
//        std::cout << "message: " << m << std::endl;
//        ASSERT_FALSE(true);
//    });
//
//    // then published message is not received by subscriber.
//    t.join();
//    EXPECT_EQ(number_of_messages_received, 0);
//}
//
//TEST_F(MqttTest, retained_published_messages_are_received_directly_after_subscription) {
//    // given connected mqtt client A.
//    boost::asio::io_context ioc;
//    MqttClient mqttClient{"127.0.0.1", 1883,"hello",  ioc};
//    // and ioc run
//    auto t = std::thread([&ioc](){
//        ioc.run();
//    });
//    sleep(1);
//
//    // and mqtt client A publishes to "hello/world" as a retained message.
//    auto option = MqttPublishOptions();
//    option.Retain = true;
//    mqttClient.Publish("hello/world", "HELLO WORLD", option);
//    sleep(1);
//
//    // when mqtt client A subscribes to "hello/world".
//    int number_of_messages_received = 0;
//    mqttClient.Subscribe("hello/world", [&number_of_messages_received, &mqttClient](const WildcardValue& wc, const Message& m){
//        EXPECT_EQ(m, "HELLO WORLD");
//        EXPECT_TRUE(wc.empty());
//        number_of_messages_received ++;
//
//        mqttClient.Disconnect();
//    });
//
//    // then retained message received by subscriber.
//    t.join();
//    EXPECT_EQ(number_of_messages_received, 1);
//}
//
//TEST_F(MqttTest, mqtt_client_receives_persistent_messages_after_subscribe) {
//    // given connected mqtt client A with persistent flag and mqtt client B.
//    boost::asio::io_context ioc;
//    MqttClient mqttClientA{"127.0.0.1", 1883, "hello1", ioc, false};
//    MqttClient mqttClientB{"127.0.0.1", 1883, "hello2", ioc};
//
//    // and ioc run
//    auto t = std::thread([&ioc](){
//        ioc.run();
//    });
//    async_sleep(ioc, 1);
//
//    // and mqtt client A subscribes to topic "hello/world" with QoS 1
//    mqttClientA.Subscribe("hello/world", [](const WildcardValue& wc, const Message& m){
//        ASSERT_TRUE(false);
//    });
//
//    // and mqtt client A disconnects
//    mqttClientA.Disconnect();
//    async_sleep(ioc, 1);
//
//    // and mqtt client B publishes to "hello/world" with QoS 1.
//    mqttClientB.Publish("hello/world", "HELLO WORLD_1");
//    mqttClientB.Publish("hello/world", "HELLO WORLD_2");
//    mqttClientB.Publish("hello/world", "HELLO WORLD_3");
//
//    // when mqtt client C connects with persistent flag with same id as client A
//    MqttClient mqttClientC{"127.0.0.1", 1883,"hello1",  ioc, false};
//
//    // and mqtt client C subscribed to "hello/world" with QoS 1.
//    auto messages = std::vector <std::string>();
//    mqttClientC.Subscribe("hello/world", [&messages, &mqttClientC, &mqttClientB](const WildcardValue& wc, const Message& m){
//        messages.emplace_back(m);
//        EXPECT_TRUE(wc.empty());
//        if (messages.size() == 3){
//            mqttClientC.Disconnect();
//            mqttClientB.Disconnect();
//        }
//    });
//
//    // then published queued message received by subscriber.
//    t.join();
//    EXPECT_EQ(3, messages.size());
//    EXPECT_EQ(1, std::count_if(begin(messages), end(messages), [](std::string message){return message.compare("HELLO WORLD_1") == 0;}));
//    EXPECT_EQ(1, std::count_if(begin(messages), end(messages), [](std::string message){return message.compare("HELLO WORLD_2") == 0;}));
//    EXPECT_EQ(1, std::count_if(begin(messages), end(messages), [](std::string message){return message.compare("HELLO WORLD_3") == 0;}));
//}
//
////$SYS/broker/clients/connected
////        $SYS/broker/clients/disconnected
////        $SYS/broker/clients/total
////        $SYS/broker/messages/sent
////        $SYS/broker/uptime
