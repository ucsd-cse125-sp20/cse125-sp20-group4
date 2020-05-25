# CSE 125 Spring 2020 Group 4

## Package Management

We are using [vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=vs-2019) to manage external libraries in our program.

### Required Packages:

#### Client and Server:

- `spdlog:x64-windows`
- `glm:x64-windows`

#### Client

- `glew:x64-windows`
- `glfw3:x64-windows`

## External Tools

### Audio

We are using FMOD for the audio engine, which needs to be installed externally.

Download the [FMOD Studio API](https://www.fmod.com/download), install it on your computer, and set an environment variable
`FMOD_PATH` to be the path to the directory where FMOD was installed (that directory should contain the `api` directories).

We are also using the FMOD Studio program to create the sound event banks (proprietary FMOD format) being used in the project.