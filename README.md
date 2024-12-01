# `cortex`

[![Build](https://github.com/blackboxaudio/cortex/actions/workflows/ci.build.yml/badge.svg)](https://github.com/blackboxaudio/cortex/actions/workflows/ci.build.yml)
[![cortex: v0.1.0](https://img.shields.io/badge/Version-v0.1.0-blue.svg)](https://github.com/blackboxaudio/cortex)
[![License](https://img.shields.io/badge/License-MIT-yellow)](https://github.com/blackboxaudio/cortex/blob/develop/LICENSE)

> Collection of JUCE plugin components and utilities 🧠

## Overview

`cortex` is a component and utility library based on the JUCE framework that aims to make it
easier to build Black Box Audio plugins.

## Getting Started

This library is most often used within _other_ projects, namely plugins and other JUCE applications. To get started,
add this library as a submodule to your project:

```bash
git submodule add https://github.com/blackboxaudio/cortex lib/cortex
```

Add the necessary changes to your project's `CMakeLists.txt` file:

```cmake
set(CTX_PLUGIN_BUILD ON)
add_subdirectory(lib/cortex)
include_directories(lib/cortex/src)
target_link_libraries(Plugin
    PRIVATE
        ...
        cortex)
```

Your project should build successfully while including the `cortex` library!
