import React from 'react';
import StartStopButton from './StartStopButton';
import SpeedProfiles from './SpeedProfiles';

function Controls() {
    return (
        <div className='controls-container'>
            <h1 className='controls-header'>Controls</h1>

            <SpeedProfiles />
            <StartStopButton />
        </div>
    );
}

export default Controls;