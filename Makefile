all:
	gcc -o main src/main/MIDI.c

test:
	gcc -o test src/test/test.c src/test/MIDITest.c src/main/MIDI.c

clean:
	rm main
	rm test
	