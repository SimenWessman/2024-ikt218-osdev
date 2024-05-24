extern "C" {
    #include "libc/stdint.h"
    #include "libc/stdio.h"
    #include "kernel/pit.h"
    #include "song/frequencies.h"
    #include "song/song.h"
}

void enable_speaker() 
{
    // Pseudocode for enable_speaker:
    // 1. Read the current state from the PC speaker control port.
    // 2. The control register bits are usually defined as follows:
    //    - Bit 0 (Speaker gate): Controls whether the speaker is on or off.
    //    - Bit 1 (Speaker data): Determines if data is being sent to the speaker.
    // 3. Set both Bit 0 and Bit 1 to enable the speaker.
    //    - Use bitwise OR operation to set these bits without altering others.

    // 1.
    uint8_t current_state = inb(PC_SPEAKER_PORT);
    // 3.
    outb(PC_SPEAKER_PORT, current_state | 3); // Set both Bit 0 and Bit 1 to enable the speaker.


}

void disable_speaker() 
{
    // Pseudocode for disable_speaker:
    // 1. Read the current state from the PC speaker control port.
    // 2. Clear both Bit 0 and Bit 1 to disable the speaker.
    //    - Use bitwise AND with the complement of 3 (0b11) to clear these bits.

    // 1.
    uint8_t current_state = inb(PC_SPEAKER_PORT);
    // 2. 0b11 binary 3
    outb(PC_SPEAKER_PORT, current_state & 0xFC); // Clear both Bit 0 and Bit 1 to disable the speaker.
}

void play_sound(uint32_t frequency) 
{
    // Pseudocode for play_sound:
    // 1. Check if the frequency is 0. If so, exit the function as this indicates no sound.
    // 2. Calculate the divisor for setting the PIT (Programmable Interval Timer) frequency.
    //    - The PIT frequency is a base value, typically 1.193182 MHz.
    //    - The divisor is PIT frequency divided by the desired sound frequency.
    // 3. Configure the PIT to the desired frequency:
    //    - Send control word to PIT control port to set binary counting, mode 3, and access mode (low/high byte).
    //    - Split the calculated divisor into low and high bytes.
    //    - Send the low byte followed by the high byte to the PIT channel 2 port.
    // 4. Enable the speaker (by setting the appropriate bits) to start sound generation.

    // 1. Check if the frequency is 0.
    if (frequency == 0) return;


    // 2. Calculate the divisor for setting the PIT frequency.
    uint16_t divisor = PIT_BASE_FREQUENCY / frequency;

    // 3. Configure the PIT to the desired frequency:
    outb(PIT_CMD_PORT, 0xB6);

    outb(PIT_CHANNEL2_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    // 4. Enable the speaker to start sound generation.
    uint8_t current_state = inb(PC_SPEAKER_PORT);
    current_state |= 3;
    outb(PC_SPEAKER_PORT, current_state);
}

void stop_sound() 
{
    // Pseudocode for stop_sound:
    // 1. Read the current state from the PC speaker control port.
    // 2. Clear the bit that enables speaker data to stop the sound.
    //    - Use bitwise AND with the complement of the bit responsible for enabling the speaker data.

    // 1.
    uint8_t current_state = inb(PC_SPEAKER_PORT);

    // 2.
    outb(PC_SPEAKER_PORT, current_state & ~0b10); // Clear the bit that enables speaker data to stop the sound.
}

void play_song_impl(Song *song) 
{
    // Pseudocode for play_song_impl:
    // 1. Enable the speaker before starting the song.
    // 2. Loop through each note in the song's notes array:
    //    a. For each note, display its details such as frequency and duration.
    //    b. Call play_sound with the note's frequency.
    //    c. Delay execution for the duration of the note (this can be implemented with a sleep function).
    //    d. Call stop_sound to end the note.
    // 3. Disable the speaker after all notes have been played.

    // 1. Enable the speaker before starting the song.
    enable_speaker();

    // 2. Loop through each note in the song's notes array:
    for (int i = 0; i < song->length; i++) 
    {
        // a. Display note details:
        printf("Playing note %d: Frequency=%d, Duration=%d\n", i, song->notes[i].frequency, song->notes[i].duration);

        // b. Play the note:
        play_sound(song->notes[i].frequency);

        // c. Delay execution for the duration of the note:
        sleep_interrupt(song->notes[i].duration);

        // d. Stop the note:
        stop_sound();
    }

    // 3. Disable the speaker after all notes have been played.
    disable_speaker();
}

void play_song(Song *song) 
{
    // Pseudocode for play_song:
    // 1. Call play_song_impl with the given song.
    //    - This function handles the entire process of playing each note in the song.

    // 1. Call play_song_impl with the given song.
    play_song_impl(song);
}

SongPlayer* create_song_player() 
{
    auto* player = new SongPlayer();
    player->play_song = play_song_impl;
    return player;
}