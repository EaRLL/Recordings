#include "stdafx.h"
#include "Recordings.h"
using namespace std;

RecordCommand::RecordCommand ( ){}
RecordCommand::~RecordCommand ( ){}


/*
 * ��� ������ ��������� ����� ���������������� ������ � ����� �����.
 * �� ����� ������ �� ����� ���������� �.�. "�����������", ������ ��� ����������� ��������.
 * ���� ������ ������ (�������������� ������) ��� �����������,
 * ��� ��������� ������ ������ ��������� � �������� ����� ������, � � ����������� ��� � �������� ���� ��� �����������.
 * � ������ ������ ��������� �������� "Recording << " � "Stop Recording >> " �������, ������ ��� ��� ������� ��� ��������
 * ����� � ��������� ��� ��� �����������.
 * ��� �� � ���� ���� ��������� ������������������ ������ - ��� ������� �� ������ ���� � ��������.
 * � �������� - � ����� ���� �������� ����� ��� �����, ������ ��������� � ���������� ������� ������ � ����� (��������������) ������ ��� ������.
 */

void RecordCommand::ProcessRecord ( )
{
	pFormat.wFormatTag = WAVE_FORMAT_PCM;
	pFormat.nChannels = Channels;
	pFormat.wBitsPerSample = BitsPerSample;
	pFormat.nSamplesPerSec = Rate;
	pFormat.nAvgBytesPerSec = Rate * pFormat.nChannels * pFormat.wBitsPerSample / 8;
	pFormat.nBlockAlign = pFormat.nChannels * pFormat.wBitsPerSample / 8;
	pFormat.cbSize = 0;
	WaveIn = new short int[ pFormat.nAvgBytesPerSec * RecSeconds ];
	memset ( WaveIn, 0, pFormat.nAvgBytesPerSec * RecSeconds );

	RecordPr.RecordInitalized = false;
	waveInOpen ( &hWaveIn, WAVE_MAPPER, &pFormat, ( DWORD ) staticwaveInProc, (DWORD) this, CALLBACK_FUNCTION );
	//waveInOpen ( &hWaveIn, WAVE_MAPPER, &pFormat, NULL, NULL, WAVE_FORMAT_DIRECT );

	WORD BufLen = pFormat.nBlockAlign * 1024;
	hBuf = GlobalAlloc ( GMEM_MOVEABLE | GMEM_SHARE, BufLen );
	PVOID Buf = GlobalLock ( hBuf );

	WaveInHdr.lpData = ( LPSTR ) WaveIn;
	//WaveInHdr.lpData = ( LPSTR ) Buf;
	//WaveInHdr.dwBufferLength = pFormat.nAvgBytesPerSec * RecSeconds;
	WaveInHdr.dwBufferLength = pFormat.nBlockAlign * 1024;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0L;
	WaveInHdr.dwFlags = 0L;
	//WaveInHdr.dwFlags = WHDR_BEGINLOOP;
	WaveInHdr.dwLoops = 0L;
	waveInPrepareHeader ( hWaveIn, &WaveInHdr, sizeof( WAVEHDR ) );
	waveInAddBuffer ( hWaveIn, &WaveInHdr, sizeof( WAVEHDR ) );

	waveInStart ( hWaveIn );

}

void CALLBACK RecordCommand::staticwaveInProc ( HWAVEIN waveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 )
{
	reinterpret_cast<RecordCommand *>( dwParam1 )->waveInProc ( waveIn, uMsg, dwInstance, dwParam1, dwParam2 );
}

auto Cstart = std::chrono::high_resolution_clock::now ( );
auto Cstop = Cstart;
__int64 ttimer = 0;

void CALLBACK RecordCommand::waveInProc ( HWAVEIN waveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 )
{
	if ( uMsg == MM_WIM_DATA )
	{
		LPWAVEHDR mBuffer = ( WAVEHDR * ) dwParam1;

		if ( strlen ( mBuffer->lpData ) > 200 )
		{
			cout << "Tick << " << strlen ( mBuffer->lpData ) << endl;
		}

		/* �������� ������ ������, ���� �.�. "�����������" ���������� ����� 400
		 * �������������� ����� �������, ��� �� ����� ���� ������� ����� ����������� �������
		 */
		if ( strlen ( mBuffer->lpData ) > 400 && RecordPr.RecordInitalized == false )
		{
			cout << "--------------- Recording << " << strlen ( mBuffer->lpData ) << endl;

			RecordPr.RecordInitalized = true;
			RecordPr.ProcessRecord ( );
		}
			
		//ttimer = std::chrono::duration_cast< std::chrono::microseconds >( std::chrono::high_resolution_clock::now ( ) - Cstart ).count ( );

		/* ���������� ������ ������, ���� �.�. "�����������" ���������� ���� 300
		* ����������� ������ ���������� ������� � ������� ����� �� �����
		*/
		if ( strlen ( mBuffer->lpData ) < 300 && RecordPr.RecordInitalized == true )
		{
			//cout << "--------------- Stop Recording << " << strlen ( mBuffer->lpData ) << endl;
			RecordPr.RecordInitalized = false;
			RecordPr.StopRecord ( );
		}
	}

	waveInAddBuffer ( waveIn, ( WAVEHDR * ) dwParam1, sizeof( WAVEHDR ) );

}

RecordProcess::RecordProcess ( ){}
RecordProcess::~RecordProcess ( ){}

void RecordProcess::ProcessRecord ( )
{
	Cstart = std::chrono::high_resolution_clock::now ( );
	//while ( !ReordInitalized )
	//{
	//	ttimer = std::chrono::duration_cast< std::chrono::seconds >( Cstop - Cstart ).count ( );
	//}
}

void RecordProcess::StopRecord ( )
{
	RecordInitalized = false;
	Cstop = std::chrono::high_resolution_clock::now ( );
	cout << "--------------- Stop Recording >> " << std::chrono::duration_cast<std::chrono::microseconds>( Cstop - Cstart ).count ( ) << " msec" << endl;

}

int _tmain(int argc, _TCHAR* argv[])
{
	RecordCommand Record;

	Record.Rate = 44100; // ������� �������������
	Record.Channels = 2; // ���������� ������� (1 - mono, 2 - stereo)
	Record.BitsPerSample = 16; // �������������
	Record.RecSeconds = 1; // ������ ������ // ��������� � ���� ���������

	cout << "Ticks ..." << endl;
	Record.ProcessRecord ( );
	char xs[256];
	cin.get ( xs, 256 );
	string xxs;
	xxs = xs;
	if ( xs == "exit" )
	{
		//cout << "Playing end" << endl;
		waveInReset ( Record.hWaveIn );
		waveInUnprepareHeader ( Record.hWaveIn, &Record.WaveInHdr, sizeof( WAVEHDR ) );
		waveInClose ( Record.hWaveIn );
		GlobalUnlock ( Record.hBuf );
		GlobalFree ( Record.hBuf );
		return 0;
	}
	
}
