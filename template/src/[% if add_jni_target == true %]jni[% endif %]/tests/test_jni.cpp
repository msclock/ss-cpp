#include <gtest/gtest.h>
#include "absl/log/globals.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "jvm_util.hpp"

TEST(jvm, ConstructHelloWordString) {
    jvm::Jvm::instance().call([](JNIEnv* env) {
        // Construct a String
        jstring jstr = env->NewStringUTF("Hello World");

        // Get the method that you want to call
        jmethodID to_lower =
            env->GetMethodID(env->FindClass("java/lang/String"), "toLowerCase", "()Ljava/lang/String;");

        // Call the method on the object
        jobject result = env->CallObjectMethod(jstr, to_lower);

        // Get a C-style string
        const char* str = env->GetStringUTFChars((jstring)result, NULL);

        LOG(INFO) << str;

        // Clean up
        env->ReleaseStringUTFChars(jstr, str);
    });
}

TEST(jvm, CallMainJar) {
    jvm::Jvm::instance().call([](JNIEnv* env) {
        jclass j_my_test = env->FindClass("MyTest");
        jmethodID j_mymain = env->GetStaticMethodID(j_my_test, "mymain", "()V");
        if (j_my_test && j_mymain)
            env->CallStaticVoidMethod(j_my_test, j_mymain);
    });
}
