
#include "struct.h"
#include <gtest/gtest.h>

//traceFile= fopen("sample-A.dmp","rb");

TEST(Test, createIP) { 
    ASSERT_EQ(TRUE, createIP(255));
    ASSERT_EQ(FALSE,createIP(-2));

}


TEST(Test,connection ) {
   double start_ts=123.255, current_ts=0;
   unsigned char protocol=127;
   struct connectionNode connection;
   memset(&connection,0,sizeof(struct connectionNode));
   ASSERT_EQ(TRUE,printConnection(&connection));
}


TEST(Test,printRTT ) {
   double start_ts=123.255, current_ts=0;
   unsigned char protocol=127;
   struct connectionNode connection;
   memset(&connection,0,sizeof(struct connectionNode));
   ASSERT_EQ(TRUE,printRTT(&connection));
}


TEST(Test,safeFRead ) {
    int ptr;
    size_t size = 1;
    size_t nobj = 12;
    FILE * fptr=fopen("sample-A.dmp","rb");
    ASSERT_EQ(TRUE, safeFRead(&ptr, size, nobj, fptr));
}

TEST(Test, next_packet) {
    struct packetInfo pkts;
    memset(&pkts,0,sizeof(struct packetInfo));
    ASSERT_EQ(TRUE,next_packet(&pkts));
}

TEST(Test, next_usable_packet) {
    struct packetInfo pkts;
    memset(&pkts,0,sizeof(struct packetInfo));
    ASSERT_EQ(TRUE,next_usable_packet(&pkts));
}

TEST(Test, next_usable_packet_tcp) {
    struct packetInfo pkts;
    memset(&pkts,0,sizeof(struct packetInfo));
    ASSERT_EQ(TRUE,next_usable_packet_tcp(&pkts));
}

TEST(Test, populateConnectionTable) {
    ASSERT_EQ(TRUE,populateConnectionTable());
}


TEST(Test, transportPacketDumping) {
    ASSERT_EQ(TRUE,transportPacketDumping());
}


TEST(Test, populateTCPTable) {
    ASSERT_EQ(TRUE,populateTCPTable());
}


TEST(Test, depopulateTable) {
    ASSERT_EQ(TRUE,depopulateTable());
}


TEST(Test, printConnectionSummaries) {
    ASSERT_EQ(TRUE,printConnectionSummaries());
}


TEST(Test, calculateRoundTripTimes) {
    ASSERT_EQ(TRUE,calculateRoundTripTimes());
}




int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}

