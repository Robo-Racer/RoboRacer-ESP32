import React from 'react';
import StartStopButton from './StartStopButton';
import SpeedProfiles from './SpeedProfiles';

function Controls() {
    return (
        <div className='controls-container'>
            <h1>Controls</h1>

            {/* <div className='start-container'> */}
                <SpeedProfiles className='inputs-container' />
                <StartStopButton className='start-stop-container' />
            {/* </div> */}
        </div>
    );
}

export default Controls;