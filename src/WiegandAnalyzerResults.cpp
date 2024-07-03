#include "WiegandAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "WiegandAnalyzer.h"
#include "WiegandAnalyzerSettings.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>



WiegandAnalyzerResults::WiegandAnalyzerResults( WiegandAnalyzer* analyzer, WiegandAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

WiegandAnalyzerResults::~WiegandAnalyzerResults()
{
}
void WiegandAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );
	char number_str[128];

    U32 tempMaxBits = std::max( mSettings->facility_length, mSettings->cardid_length);
    U32 maxBits = std::max( tempMaxBits, mSettings->site_length);

    AnalyzerHelpers::GetNumberString( frame.mData1, display_base, maxBits, number_str, 128 );


    if( frame.mType == 1 || frame.mType == 5 )
    {
        std::stringstream ss;

		ss << "P[" << number_str << "]";
        AddResultString( ss.str().c_str() );
        ss.str( "" );

        ss << "Parity[" << number_str << "]";
        AddResultString( ss.str().c_str() );
    }
    else if( frame.mType == 2 )
    {
        std::stringstream ss;

        ss << "S[" << number_str << "]";
        AddResultString( ss.str().c_str() );
        ss.str( "" );

		ss << "Site[" << number_str << "]";
        AddResultString( ss.str().c_str() );
    }
    else if( frame.mType == 3 )
    {
        std::stringstream ss;
        ss << "F[" << number_str << "]";
        AddResultString( ss.str().c_str() );
        ss.str( "" );

        ss << "Facility[" << number_str << "]";
        AddResultString( ss.str().c_str() );
    
	}
    else if( frame.mType == 4 )
    {
        std::stringstream ss;
        ss << "C[" << number_str << "]";
        AddResultString( ss.str().c_str() );
        ss.str( "" );

        ss << "Card[" << number_str << "]";
        AddResultString( ss.str().c_str() );
    


    }
	
}

void WiegandAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void WiegandAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
   
#endif
}

void WiegandAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void WiegandAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}