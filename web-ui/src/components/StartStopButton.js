import React from 'react';
import Button from '@mui/material/Button';

function StartStopButton() {

    function handleStart() {
        console.log("Start RoboRacer");
    }

    function handleStop() {
        console.log("Stop RoboRacer");
    }

    return (
        <div className='start-stop-container'>
            <Button
                variant='contained'
                sx={{ m: 5, width: 150, height: 150, fontSize: 30 }}
                color='success'
                onClick={handleStart}>
                Start</Button>
            <Button
                variant='contained'
                sx={{ m: 5, width: 150, height: 150, fontSize: 30 }}
                color='error'
                onClick={handleStop}>
                Stop</Button>
        </div>
    );
}

export default StartStopButton;