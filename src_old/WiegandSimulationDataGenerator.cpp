#include "WiegandSimulationDataGenerator.h"
#include "WiegandAnalyzerSettings.h"

#include <AnalyzerHelpers.h>


WiegandSimulationDataGenerator::WiegandSimulationDataGenerator()
{
}

WiegandSimulationDataGenerator::~WiegandSimulationDataGenerator()
{
}

void WiegandSimulationDataGenerator::Initialize( U32 simulation_sample_rate, WiegandAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mSerialSimulationData.SetChannel( mSettings->D0Channel );
	mSerialSimulationData.SetSampleRate( simulation_sample_rate );
	mSerialSimulationData.SetInitialBitState( BIT_HIGH );
}

U32 WiegandSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mSerialSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		CreateSerialByte();
	}

	*simulation_channel = &mSerialSimulationData;
	return 1;
}

void WiegandSimulationDataGenerator::CreateSerialByte()
{

	U8 byte = mSerialText[ mStringIndex ];
	mStringIndex++;
	if( mStringIndex == mSerialText.size() )
		mStringIndex = 0;

	//we're currenty high
	//let's move forward a little

	mSerialSimulationData.Transition();  //low-going edge for start bit

	U8 mask = 0x1 << 7;
	for( U32 i=0; i<8; i++ )
	{
		if( ( byte & mask ) != 0 )
			mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
		else
			mSerialSimulationData.TransitionIfNeeded( BIT_LOW );

		mask = mask >> 1;
	}

	mSerialSimulationData.TransitionIfNeeded( BIT_HIGH ); //we need to end high

	//lets pad the end a bit for the stop bit:
}
