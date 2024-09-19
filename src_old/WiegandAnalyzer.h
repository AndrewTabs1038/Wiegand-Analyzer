#ifndef WIEGAND_ANALYZER_H
#define WIEGAND_ANALYZER_H

#include <Analyzer.h>
#include "WiegandAnalyzerResults.h"
#include "WiegandSimulationDataGenerator.h"

class WiegandAnalyzerSettings;
class ANALYZER_EXPORT WiegandAnalyzer : public Analyzer2
{
public:
	WiegandAnalyzer();
	virtual ~WiegandAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< WiegandAnalyzerSettings > mSettings;
	std::auto_ptr< WiegandAnalyzerResults > mResults;
	AnalyzerChannelData* D0Serial;
	AnalyzerChannelData* D1Serial;

	WiegandSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //WIEGAND_ANALYZER_H
