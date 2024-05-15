import React from 'react';

function StartStopButton() {

    function handleStart() {
        console.log("Start RoboRacer");
    }

    function handleStop() {
        console.log("Stop RoboRacer");
    }

    return (
        <div className='start-stop-container'>
            <button
                onClick={handleStart}
                className='start-button'>
                START</button>
            <button
                onClick={handleStop}
                className='stop-button'>
                STOP</button>
        </div>
    );
}

export default StartStopButton;