import React from 'react';

function StartStopButton() {

    function handleStart() {
        console.log("Start RoboRacer");

        fetch('/start', {
            method: 'POST',
            body: {
                directive: 'start'
            } 
        })
    }

    function handleStop() {
        console.log("Stop RoboRacer");

        fetch('/stop', {
            method: 'POST',
            body: {
                directive: 'stop'
            }
        })
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