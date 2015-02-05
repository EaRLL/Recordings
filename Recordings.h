#pragma once

#include "stdafx.h"

//bool Stop;

class RecordMcv
{
public:
	int Rate;
	int Channels;
	int BitsPerSample;
	int RecSeconds;
	short int *WaveIn;

	HANDLE hBuf;
	WAVEFORMATEX pFormat;
	HWAVEIN hWaveIn;
	WAVEHDR WaveInHdr;
	HWAVEOUT hWaveOut;
	WAVEHDR WaveOutHdr;
};

class RecordProcess : public RecordMcv
{
public:
	RecordProcess ( );
	~RecordProcess ( );

	bool RecordInitalized = false;

	void ProcessRecord ( );
	void StopRecord ( );
};

class RecordCommand : public RecordMcv
{
public:
	RecordCommand ( );
	~RecordCommand ( );

	RecordProcess RecordPr;

	void ProcessRecord ( );
	void waveOutPut ( LPSTR waveIn );
	static void CALLBACK staticwaveInProc ( HWAVEIN waveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 );
	void CALLBACK waveInProc ( HWAVEIN waveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 );
};
