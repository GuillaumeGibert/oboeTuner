#include "workers/WorkerMusicPlayer.h"

WorkerMusicPlayer::WorkerMusicPlayer()
{
    m_fFps = 0.0;
    m_fDuration = 0.0;
    m_i32NbSamples = 0;
    m_pAudioOutput = nullptr;
    m_pByteBuffer = nullptr;
	
    m_bIsWorkerInitialized = false;
}

WorkerMusicPlayer::WorkerMusicPlayer(float fSampleRate, float fDuration)
{
    m_fFps = fSampleRate;
    m_fDuration = fDuration;
    m_i32NbSamples = fSampleRate * fDuration;
    m_pAudioOutput = nullptr;
    m_pByteBuffer = nullptr;

    m_bIsWorkerInitialized = false;

    init();
}

WorkerMusicPlayer::~WorkerMusicPlayer()
{
    /*m_pAudioOutput->stop();
    if (nullptr != m_pAudioOutput)
    {
        delete m_pAudioOutput;
        m_pAudioOutput = nullptr;
    }*/

    if (nullptr != m_pByteBuffer)
    {
        delete m_pByteBuffer;
        m_pByteBuffer = nullptr;
    }
}

void WorkerMusicPlayer::doWork()
{

}

bool WorkerMusicPlayer::init()
{
    // checks if sample rate was correctly set
    if (m_fFps == 0.0)
    {
        qWarning() << "(WorkerMusicPlayer::init) Sample rate was not correctly set.";
        m_bIsWorkerInitialized = false;
        return m_bIsWorkerInitialized;
    }

    // checks if duration was correctly set
    if (m_fDuration == 0.0)
    {
        qWarning() << "(WorkerMusicPlayer::init) Duration was not correctly set.";
        m_bIsWorkerInitialized = false;
        return m_bIsWorkerInitialized;
    }

    // creates and setups a QAudioFormat object
    m_oAudioFormat.setSampleRate(static_cast<int>(m_fFps));
    m_oAudioFormat.setChannelCount(1);
    m_oAudioFormat.setSampleSize(32);   // set the sample size in bits. We set it to 32 bis, because we set SampleType to float (one float has 4 bytes ==> 32 bits)
    m_oAudioFormat.setCodec("audio/pcm");
    m_oAudioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_oAudioFormat.setSampleType(QAudioFormat::Float);   // use Float, to have a better resolution than SignedInt or UnSignedInt

     // creates a QAudioDeviceInfo object, to make sure that our audioFormat is supported by the device
    QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    if (!deviceInfo.isFormatSupported(m_oAudioFormat))
    {
        qWarning() << "(WorkerMusicPlayer::init) Raw audio format not supported by backend, cannot play audio.";
        m_bIsWorkerInitialized = false;
        return m_bIsWorkerInitialized;
    }

    // creates an audio output with our QAudioFormat
    if (nullptr == m_pAudioOutput)
        m_pAudioOutput = new QAudioOutput(m_oAudioFormat, this);

    m_pAudioOutput->setVolume(1.0);

    m_pByteBuffer = new QByteArray();  // create a new instance of QByteArray class (in the heap, dynamically arranged in memory), and set its pointer to byteBuffer

    return m_bIsWorkerInitialized;
}

void WorkerMusicPlayer::setSignalValues(std::vector<float> vSignalValues)
{
    m_oWorkerMutex.lockForWrite();
        m_vSignalValues = vSignalValues;
    m_oWorkerMutex.unlock();

    play();
}

void WorkerMusicPlayer::setFps(float fFps)
{
    m_oWorkerMutex.lockForWrite();
        m_fFps = fFps;
        m_i32NbSamples = m_fFps * m_fDuration;
    m_oWorkerMutex.unlock();
}

void WorkerMusicPlayer::setDuration(float fDuration)
{
    m_oWorkerMutex.lockForWrite();
        m_fDuration = fDuration;
        m_i32NbSamples = m_fFps * m_fDuration;
    m_oWorkerMutex.unlock();
}

/*
void WorkerMusicPlayer::play()
{
    // --- transfer QVector data to QByteBuffer
    m_pByteBuffer->resize(sizeof(float) * m_i32NbSamples);  // resize byteBuffer to the total number of bytes that will be needed to accommodate all the n data samples that are of type float

    for (std::vector<float>::iterator it = m_vSignalValues.begin(); it != m_vSignalValues.end(); ++it)
    {
        // breaks down one float into four bytes
        //float sample = (float)sinVal;  // save one data sample in a local variable, so I can break it down into four bytes
        //char* ptr = (char*)(&sample);  // assign a char* pointer to the address of this data sample

        float sample = *it;
        char* ptr = (char*)(&sample);  // assign a char* pointer to the address of this data sample
        char byte00 = *ptr;         // 1st byte
        char byte01 = *(ptr + 1);   // 2nd byte
        char byte02 = *(ptr + 2);   // 3rd byte
        char byte03 = *(ptr + 3);   // 4th byte

        // puts byte data into QByteArray, one byte at a time
        int index = std::distance(m_vSignalValues.begin(), it);
        (*m_pByteBuffer)[4 * index] = byte00;       // put 1st byte into QByteArray
        (*m_pByteBuffer)[4 * index + 1] = byte01;   // put 2nd byte into QByteArray
        (*m_pByteBuffer)[4 * index + 2] = byte02;   // put 3rd byte into QByteArray
        (*m_pByteBuffer)[4 * index + 3] = byte03;   // put 4th byte into QByteArray
    }

    // Make a QBuffer with our QByteArray
    QBuffer* input = new QBuffer(m_pByteBuffer);
    input->open(QIODevice::ReadOnly);   // set the QIODevice to read-only

    // connect up signal stateChanged to a lambda to get feedback
    connect(m_pAudioOutput, &QAudioOutput::stateChanged, [this, input](QAudio::State newState)
    {
        if (newState == QAudio::IdleState)   // finished playing (i.e., no more data)
        {
            qDebug() << "(WorkerMusicPlayer::play) Finished playing sound.";
            delete input;
        }
        // should also handle more states, e.g., errors. I need to figure out on how to do this later.
    });

    // start the audio (i.e., play sound from the QAudioOutput object that we just created)
    m_pAudioOutput->start(input);
    
}
*/

/*
void WorkerMusicPlayer::play()
{
    // initialize parameters
    qreal sampleRate = 8000;   // sample rate
    qreal duration = 1.000;     // duration in seconds
    qreal frequency = 1000;     // frequency
    const quint32 n = static_cast<quint32>(duration * sampleRate);   // number of data samples

    // --- transfer QVector data to QByteBuffer
    QByteArray* byteBuffer = new QByteArray();  // create a new instance of QByteArray class (in the heap, dynamically arranged in memory), and set its pointer to byteBuffer
    byteBuffer->resize(sizeof(float) * n);  // resize byteBuffer to the total number of bytes that will be needed to accommodate all the n data samples that are of type float

   
    for (quint32 i = 0; i < n; i++)
    {
        //qreal sinVal;
        qreal sinVal = (qreal)sin(2.0 * 3.14 * frequency * i / sampleRate);  // create sine wave data samples, one at a time

        // break down one float into four bytes
        float sample = (float)sinVal;  // save one data sample in a local variable, so I can break it down into four bytes
        char* ptr = (char*)(&sample);  // assign a char* pointer to the address of this data sample
        char byte00 = *ptr;         // 1st byte
        char byte01 = *(ptr + 1);   // 2nd byte
        char byte02 = *(ptr + 2);   // 3rd byte
        char byte03 = *(ptr + 3);   // 4th byte

        // put byte data into QByteArray, one byte at a time
        (*byteBuffer)[4 * i] = byte00;       // put 1st byte into QByteArray
        (*byteBuffer)[4 * i + 1] = byte01;   // put 2nd byte into QByteArray
        (*byteBuffer)[4 * i + 2] = byte02;   // put 3rd byte into QByteArray
        (*byteBuffer)[4 * i + 3] = byte03;   // put 4th byte into QByteArray
    }

    // create and setup a QAudioFormat object
   QAudioFormat audioFormat;
    audioFormat.setSampleRate(static_cast<int>(sampleRate));
    audioFormat.setChannelCount(1);
    audioFormat.setSampleSize(32);   // set the sample size in bits. We set it to 32 bis, because we set SampleType to float (one float has 4 bytes ==> 32 bits)
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::Float);   // use Float, to have a better resolution than SignedInt or UnSignedInt
    
    // create a QAudioDeviceInfo object, to make sure that our audioFormat is supported by the device
  QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    if (!deviceInfo.isFormatSupported(audioFormat))
    {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }
   
    // Make a QBuffer with our QByteArray
    QBuffer* input = new QBuffer(byteBuffer);
    input->open(QIODevice::ReadOnly);   // set the QIODevice to read-only

    // Create an audio output with our QAudioFormat
    QAudioOutput* audio = new QAudioOutput(audioFormat, this);

    // connect up signal stateChanged to a lambda to get feedback
    connect(audio, &QAudioOutput::stateChanged, [audio, input](QAudio::State newState)
        {
            if (newState == QAudio::IdleState)   // finished playing (i.e., no more data)
            {
                qDebug() << "finished playing sound";
                delete audio;
                delete input;
                //delete byteBuffer;  // I tried to delete byteBuffer pointer (because it may leak memories), but got compiler error. I need to figure this out later.
            }
            // should also handle more states, e.g., errors. I need to figure out on how to do this later.
        });

    // start the audio (i.e., play sound from the QAudioOutput object that we just created)
    audio->start(input);
}
*/

void WorkerMusicPlayer::play()
{
    // initialize parameters
   // qreal sampleRate = 8000;   // sample rate
   // qreal duration = 1.000;     // duration in seconds
   // qreal frequency = 261;     // frequency
   // const quint32 n = static_cast<quint32>(duration * sampleRate);   // number of data samples
    //const quint32 n = m_i32NbSamples;

    // --- transfer QVector data to QByteBuffer
    //QByteArray* byteBuffer = new QByteArray();  // create a new instance of QByteArray class (in the heap, dynamically arranged in memory), and set its pointer to byteBuffer
    //byteBuffer->resize(sizeof(float) * m_i32NbSamples/*n*/);  // resize byteBuffer to the total number of bytes that will be needed to accommodate all the n data samples that are of type float
    m_pByteBuffer->resize(sizeof(float) * m_i32NbSamples);
    for (std::vector<float>::iterator it = m_vSignalValues.begin(); it != m_vSignalValues.end(); ++it)
    //for (quint32 i = 0; i < n; i++)
    {
        //qreal sinVal;
        //qreal sinVal = (qreal)sin(2.0 * 3.14 * frequency * i / sampleRate);  // create sine wave data samples, one at a time

        // break down one float into four bytes
        //float sample = (float)sinVal;  // save one data sample in a local variable, so I can break it down into four bytes
        //float sample = m_vSignalValues[i];
        float sample = *it;
        char* ptr = (char*)(&sample);  // assign a char* pointer to the address of this data sample
        char byte00 = *ptr;         // 1st byte
        char byte01 = *(ptr + 1);   // 2nd byte
        char byte02 = *(ptr + 2);   // 3rd byte
        char byte03 = *(ptr + 3);   // 4th byte

        // put byte data into QByteArray, one byte at a time
        int i = std::distance(m_vSignalValues.begin(), it);
        (*m_pByteBuffer)[4 * i] = byte00;       // put 1st byte into QByteArray
        (*m_pByteBuffer)[4 * i + 1] = byte01;   // put 2nd byte into QByteArray
        (*m_pByteBuffer)[4 * i + 2] = byte02;   // put 3rd byte into QByteArray
        (*m_pByteBuffer)[4 * i + 3] = byte03;   // put 4th byte into QByteArray
    }

    // create and setup a QAudioFormat object
   /*QAudioFormat audioFormat;
    audioFormat.setSampleRate(static_cast<int>(m_fFps));
    audioFormat.setChannelCount(1);
    audioFormat.setSampleSize(32);   // set the sample size in bits. We set it to 32 bis, because we set SampleType to float (one float has 4 bytes ==> 32 bits)
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::Float);   // use Float, to have a better resolution than SignedInt or UnSignedInt

    // create a QAudioDeviceInfo object, to make sure that our audioFormat is supported by the device
  QAudioDeviceInfo deviceInfo(QAudioDeviceInfo::defaultOutputDevice());
    if (!deviceInfo.isFormatSupported(audioFormat))
    {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }*/

    // Make a QBuffer with our QByteArray
    QBuffer* input = new QBuffer(m_pByteBuffer);
    input->open(QIODevice::ReadOnly);   // set the QIODevice to read-only

    // Create an audio output with our QAudioFormat
    QAudioOutput* audio = new QAudioOutput(m_oAudioFormat, this);

    // connect up signal stateChanged to a lambda to get feedback
    connect(audio, &QAudioOutput::stateChanged, [audio, input](QAudio::State newState)
        {
            if (newState == QAudio::IdleState)   // finished playing (i.e., no more data)
            {
                qDebug() << "finished playing sound";
                delete audio;
                delete input;
                //delete byteBuffer;  // I tried to delete byteBuffer pointer (because it may leak memories), but got compiler error. I need to figure this out later.
            }
            // should also handle more states, e.g., errors. I need to figure out on how to do this later.
        });

    // start the audio (i.e., play sound from the QAudioOutput object that we just created)
    audio->start(input);
}


