#include "WiegandAnalyzer.h"
#include "WiegandAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

//Information about Wiegand here:
//https://www.paxton-access.com/wp-content/uploads/2019/03/AN1010.pdf


WiegandAnalyzer::WiegandAnalyzer()
:	Analyzer2(),  
	mSettings( new WiegandAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

WiegandAnalyzer::~WiegandAnalyzer()
{
	KillThread();
}

void WiegandAnalyzer::SetupResults()
{
	mResults.reset( new WiegandAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->D0Channel );
}

void WiegandAnalyzer::WorkerThread()
{	

    U32 site_length = mSettings->site_length;

    U32 facility_length = mSettings->facility_length;

    U32 cardid_length = mSettings->cardid_length;
   
        
    bool parity_one_done = false;
    bool done_site = false;
    bool done_facility = false;
    bool done_card = false;
    bool parity_two_done = false;

    D0Serial = GetAnalyzerChannelData( mSettings->D0Channel );
    D1Serial = GetAnalyzerChannelData( mSettings->D1Channel );


     U64 ending_pos = 0;
     U64 starting_pos = 0;


      U64 firstposition = 0;
      U64 lastposition = 0;


	U64 data = 0;
	for( ; ; )
	{
       
        if( !parity_one_done )
        {
            data = GatherBits( 1, starting_pos, ending_pos );
            parity_one_done = true;
            RecordFrame( starting_pos, ending_pos, 1, data );
        }     

        if( !done_site && !site_length == 0)
        {
            data = GatherBits( site_length, starting_pos, ending_pos );
            done_site = true;
            RecordFrame( starting_pos, ending_pos, 2, data );
        }
        if( !done_facility && !facility_length == 0 )
        {
            data = GatherBits( facility_length, starting_pos, ending_pos );
            done_facility = true;
            RecordFrame( starting_pos, ending_pos, 3, data );
        }

        if( !done_card && !cardid_length == 0 )
        {
            data = GatherBits( cardid_length, starting_pos, ending_pos );
            done_card = true;
            RecordFrame( starting_pos, ending_pos, 4, data );

        }

        if( !parity_two_done )
        {
            data = GatherBits( 1, starting_pos, ending_pos );
            parity_two_done = true;
            RecordFrame( starting_pos, ending_pos, 1, data );
        }

        if( D0Serial->DoMoreTransitionsExistInCurrentData() || D1Serial->DoMoreTransitionsExistInCurrentData() )
        {
            parity_one_done = false;
            done_site = false;
            done_facility = false;
            done_card = false;
            parity_two_done = false;
        }
		CheckIfThreadShouldExit();

        
	}
}

void WiegandAnalyzer::RecordFrame( U64 starting_sample, U64 ending_sample, U8 type, U64 data)
{
    Frame frame;
    frame.mData1 = data;
    frame.mFlags = 0;
    frame.mType = type;
    frame.mStartingSampleInclusive = starting_sample;
    frame.mEndingSampleInclusive = ending_sample;

    mResults->AddFrame( frame );
    mResults->CommitResults();
    ReportProgress( frame.mEndingSampleInclusive );

}

U64 WiegandAnalyzer::GatherBits(U32 length, U64& starting_pos, U64& ending_pos)
{
    U64 data = 0;

    U64 positionD0 = 0;
    U64 positionD1 = 0;
    bool D0Transitions = true;
    bool D1Transitions = true;       
    bool ignore_D0 = false;
    bool ignore_D1 = false;

    for( U32 i = 0; i < length; i++ )
    {
        D0Transitions = D0Serial->DoMoreTransitionsExistInCurrentData();
        D1Transitions = D1Serial->DoMoreTransitionsExistInCurrentData();
        if( !( D0Transitions || D1Transitions ) )
            return 0;
        if( D0Transitions )
            positionD0 = D0Serial->GetSampleOfNextEdge();
        else
            ignore_D0 = true;

        if( D1Transitions )
            positionD1 = D1Serial->GetSampleOfNextEdge();
        else
            ignore_D1 = true;

        if( positionD1 < positionD0 && !ignore_D1)
        {
            D1Serial->AdvanceToNextEdge();
            if( i == 0 )
                starting_pos = D1Serial->GetSampleNumber();
            D1Serial->AdvanceToNextEdge();
            if( i == length - 1 )
                ending_pos = D1Serial->GetSampleNumber();
            mResults->AddMarker( D1Serial->GetSampleNumber(), AnalyzerResults::One, mSettings->D1Channel );
            data |= static_cast<U64>( 1 ) << ( length - i - 1 );
        }
        else if(!ignore_D0)
        {
            D0Serial->AdvanceToNextEdge();
            if( i == 0 )
                starting_pos = D0Serial->GetSampleNumber();
            D0Serial->AdvanceToNextEdge();
            if( i == length - 1 )
                ending_pos = D0Serial->GetSampleNumber();
            mResults->AddMarker( D0Serial->GetSampleNumber(), AnalyzerResults::Zero, mSettings->D0Channel );
            
        }
       
        
    }
    return data;
}


bool WiegandAnalyzer::NeedsRerun()
{
	return false;
}

U32 WiegandAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 WiegandAnalyzer::GetMinimumSampleRateHz()
{
    return 25000;
}

const char* WiegandAnalyzer::GetAnalyzerName() const
{
	return "Wiegand";
}

const char* GetAnalyzerName()
{
	return "Wiegand";
}

Analyzer* CreateAnalyzer()
{
	return new WiegandAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
