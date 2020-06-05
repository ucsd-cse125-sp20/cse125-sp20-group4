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

## Assets

Some 3rd-party assets were used in this project.

### Sounds

- Background Music: [`Next to You` by `Joth`](https://opengameart.org/content/next-to-you) ([CC0 1.0 Universal Public Domain](https://creativecommons.org/publicdomain/zero/1.0/))
- Player Walk: [`Footsteps - concrete - OD - B.wav` by `FreqMan`](https://freesound.org/people/FreqMan/sounds/31368/) ([CC Attribution 3.0 License](https://creativecommons.org/licenses/by/3.0/))
- Enemy Walk: [`01041 footsteps on heels 4.wav` by `Robinhood76`](https://freesound.org/people/Robinhood76/sounds/72349/) ([CC Attribution Noncommercial 3.0 License](http://creativecommons.org/licenses/by-nc/3.0/))
- Enemy Leave: [`Pop 3` by `greenvwbeetle`](https://freesound.org/people/greenvwbeetle/sounds/244653/) ([CC0 1.0 Universal Public Public Domain](https://creativecommons.org/publicdomain/zero/1.0/))
- Enemy Goal: [`Toilet Paper.mp3` by `Caitlin_100`](https://freesound.org/people/Caitlin_100/sounds/365541/) ([CC0 1.0 Universal Public Public Domain](https://creativecommons.org/publicdomain/zero/1.0/))
- Item Purchase: [`Cash Register` by `kiddpark`](https://freesound.org/people/kiddpark/sounds/201159/) ([CC Attribution 3.0 License](https://creativecommons.org/licenses/by/3.0/))
- Barricade Break: [`Smashing, Wooden Fence, C.wav` by `InspectorJ`](https://freesound.org/people/InspectorJ/sounds/352208/) ([CC Attribution 3.0 License](https://creativecommons.org/licenses/by/3.0/))
- Barricade Place: [`wooden object set on table 4` by `youandbiscuitme`](https://freesound.org/people/youandbiscuitme/sounds/258245/) ([CC Attribution 3.0 License](https://creativecommons.org/licenses/by/3.0/))
- Use Blue Item (Jug): [`Drinking liquid` by `Breviceps`](https://freesound.org/people/Breviceps/sounds/445970/) ([CC0 1.0 Universal Public Public Domain](https://creativecommons.org/publicdomain/zero/1.0/))
- Use Red Item (Bottle): [`Opening a bottle of beer.WAV` by `13GPanska_Gorbusinova_Anna`](https://freesound.org/people/13GPanska_Gorbusinova_Anna/sounds/377991/) ([CC0 1.0 Universal Public Public Domain](https://creativecommons.org/publicdomain/zero/1.0/))
- Use Green Item (Watermelon): [`Eating Watermelon` by `alirabiei`](https://freesound.org/people/alirabiei/sounds/491835/) ([CC Attribution 3.0 License](https://creativecommons.org/licenses/by/3.0/))
- Game Over: [Wilhelm Scream](https://commons.wikimedia.org/wiki/File:Wilhelm_Scream.ogg) (Public Domain)