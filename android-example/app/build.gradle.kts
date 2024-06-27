plugins {
    id("com.android.application")
    id("org.jetbrains.kotlin.android")
}

android {
    namespace = "io.cambium.webrtc.srtp"
    compileSdk = 32
    defaultConfig {
        applicationId = "io.cambium.webrtc.srtp"
        minSdk = 32
        //When using native components, you are not supposed to specify the target SDK:
        // it always uses the minSdk value anyway, and so Android Studio complains if you set it.
        //targetSdk = 32
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        //We don't have C++ code ourselves, but this is needed to tell the build
        // that it has native components that it needs to reference
        externalNativeBuild {
            cmake {
                arguments += "-DANDROID_STL=c++_shared"
                arguments += "-DANDROID_ABI=arm64-v8a"
            }
        }
        ndk {
            abiFilters += "arm64-v8a"
            stl = "c++_shared"

        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_1_8
        targetCompatibility = JavaVersion.VERSION_1_8
    }
    kotlinOptions {
        jvmTarget = "1.8"
    }
}

dependencies {
    implementation("androidx.core:core-ktx:1.8.0")
    implementation("org.webrtc:google-webrtc:1.0.32006")
    implementation(files("src/main/jar/srtp-plugin-1.0.0.jar"))
    testImplementation("junit:junit:4.13.2")
    androidTestImplementation("androidx.test.ext:junit:1.1.5")
    androidTestImplementation("androidx.test:runner:1.2.0")
}