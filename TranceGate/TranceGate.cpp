#include "daisy_patch.h"
#include "daisysp.h"
#include "envelope.h"

using namespace daisy;
using namespace daisysp;

float      sampleRate;

DaisyPatch hw;

TranceGate::Envelope   env;

void UpdateControls()
{
    hw.ProcessAnalogControls();
	hw.ProcessDigitalControls();
	env.SetRise(hw.GetKnobValue(DaisyPatch::CTRL_1)); // value expected to be from 0.f to 1.f
    env.SetFall(hw.GetKnobValue(DaisyPatch::CTRL_2)); // idk what this is gonna output 0-4095?

	if(hw.gate_input[DaisyPatch::GATE_IN_1].Trig())
    {
        env.Trigger();
    }
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	UpdateControls();
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = env.Process() * in[0][i]; 
		out[1][i] = env.Process() * in[1][i];
	}
}

int main(void)
{
	hw.Init();
	sampleRate = hw.AudioSampleRate();

	env.Init(sampleRate);
	hw.StartAdc();
	hw.StartAudio(AudioCallback);
	while(1) {}
}
