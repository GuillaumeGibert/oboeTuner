%function generateFakeData(nbSignals, amplitudeSignal, freqSignal, duration, samplingFreq, outFilename, plt)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%function generateFakeData(nbSignals, amplitudeSignal, freqSignal, duration, samplingFreq, outFilename, plt)
%ex.: generateFakeData(5, [1 1 1 1 1], [0.5 1 2 3 4], 2, 10, '../data/fakeData.txt', 1)
%
% Aim: To generate fake data for the Arduino Simulator
%
% Inputs:
%	- nbSignals : filter order
%	- amplitudeSignal: amplitude of the signals
%	- freqSignal: frequency of the signals (in Hz)
%	- duration: duration of the signals (in seconds)
%	- samplingFreq: sampling frequency (in Hz)
%	- outFilename: filename where to store the fake data
%	- plt: if set to 1 display the generated signals
%
% Ouput: None
%
% Guillaume Gibert 
% 09/04/2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


amplitudeSignal = 0.9;
freqSignal = 632;
duration = 4;
samplingFreq = 8000;
outFilename = 'la.wav';


% generates a series of  timestamps
t=0:1/samplingFreq:duration;

% generates the signals
generatedSignals = amplitudeSignal*sin(2*pi*freqSignal*t);
audiowrite(outFilename,generatedSignals,samplingFreq);