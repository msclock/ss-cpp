#pragma once

#include <cstdlib>
#include <functional>
#include <vector>
#include "absl/log/log.h"
#include "jni.h"
#include "jni_md.h"

// Disable leak detection from sanitizer, see
// https://stackoverflow.com/questions/51060801/how-to-suppress-leaksanitizer-report-when-running-under-fsanitize-address
#ifdef __cplusplus
extern "C" {
    const char *__asan_default_options() {
        return "detect_leaks=0";
    }

    const JavaVMInitArgs init(const char *jar_path_) {
        const jint jvm_version = JNI_VERSION_10;
        static std::string jar_path("-Djava.class.path=");
        jar_path += jar_path_;
        static std::vector<JavaVMOption> vm_options{
            JavaVMOption{
                .optionString = jar_path.data(),
                .extraInfo = nullptr,
            },
        };
        static JavaVMInitArgs vm_args;
        vm_args.version = jvm_version;       // minimum Java version
        vm_args.options = vm_options.data(); // JVM invocation options
        vm_args.nOptions = vm_options.size();
        vm_args.ignoreUnrecognized = 1; // invalid options make the JVM init fail
        return vm_args;
    }
}
#endif

namespace jvm {
class Jvm {
public:
    JNIEnv *jvm_env() { return env; }

    static Jvm &instance(const JavaVMInitArgs(init_callback)(const char *) = init,
                         const char *jar_path_ = "main_jar.jar") {
        static auto singleton = Jvm(init_callback, jar_path_);
        return singleton;
    }

    void call(std::function<void(JNIEnv *)> callback) { return callback(env); }

private:
    Jvm(const JavaVMInitArgs(init_callback)(const char *), const char *jar_path_) {
        auto vm_args = init_callback(jar_path_);
        jint res = JNI_CreateJavaVM(&vm, (void **)&env, (void *)&vm_args);
        if (res < 0) {
            LOG(INFO) << "Init JVM failed";
            exit(-1);
        }
        //=============== Display JVM version =======================================
        jint ver = env->GetVersion();
        LOG(INFO) << "Loaded JVM Version " << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f);
        // Construct a String
        jstring jstr = env->NewStringUTF("Init jvm backend successfully");
        // Get a C-style string
        const char *str = env->GetStringUTFChars((jstring)jstr, NULL);

        std::cout << str << '\n';

        // Clean up
        env->ReleaseStringUTFChars(jstr, str);
    }

    // ~Jvm() { // Shutdown the VM. Do not open it.
    //     if (vm) {
    //         vm->DestroyJavaVM();
    //     }
    //     vm = nullptr;
    // }

    Jvm(const Jvm &) = delete;
    Jvm &operator=(const Jvm &) = delete;
    JavaVM *vm{nullptr};
    JNIEnv *env{nullptr};
};

} // namespace jvm
