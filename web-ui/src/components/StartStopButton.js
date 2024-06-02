import React from 'react';

function StartStopButton() {

    function handleStart() {
        console.log("Start RoboRacer");

        const data = {
            directive: 'start'
        }

        fetch('/start', {
            method: 'POST',
            body: JSON.stringify(data)
        })
    }

    function handleStop() {
        console.log("Stop RoboRacer");

        const data = {
            directive: 'stop'
        }

        fetch('/stop', {
            method: 'POST',
            body: JSON.stringify(data)
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