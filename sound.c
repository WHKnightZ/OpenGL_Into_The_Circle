void Init_Sound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != -1) {
        Sound_Play = Mix_LoadWAV("Sounds/Play.wav");
        Sound_Shoot = Mix_LoadWAV("Sounds/Shoot.wav");
        Sound_Dead = Mix_LoadWAV("Sounds/Dead.wav");
    }
}
