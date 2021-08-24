#include "daisy_patch.h"
#include "daisysp.h"
#include "envelope.h"

using namespace daisy;
using namespace daisysp;

float      sampleRate;
float gain;
float threshold;
float lastVal;
float lastHigh;
DaisyPatch hw;

bool gate;

bool audioGate;

TranceGate::Envelope   env;

void UpdateControls()
{
    hw.ProcessAnalogControls();
	//hw.ProcessDigitalControls();
	env.SetRise(hw.GetKnobValue(DaisyPatch::CTRL_2)); // value expected to be from 0.f to 1.f
    env.SetFall(hw.GetKnobValue(DaisyPatch::CTRL_3)); // idk what this is gonna output 0-4095?
	gain = hw.GetKnobValue(DaisyPatch::CTRL_1);
	threshold = hw.GetKnobValue(DaisyPatch::CTRL_4);
	
	gate = hw.gate_input[DaisyPatch::GATE_IN_1].Trig();

	if(gate)
    {
        env.Trigger();
    }
}

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	UpdateControls();
	//float output = 0.0;
	
	for (size_t i = 0; i < size; i++) // size is number of channels two in this case
	{
		 out[0][i] = env.Process() * in[0][i];
		 out[1][i] = in[1][i]; 
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
