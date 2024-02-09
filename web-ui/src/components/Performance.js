import React from 'react';
import { useState } from 'react';

function Performance() {
    const [time, setTime] = useState(0);
    const [distance, setDistance] = useState(0);
    const [speed, setSpeed] = useState(0);
    const [acceleration, setAcceleration] = useState(0);

    return (
        <div className='performance-container'>
            <h1>Performance</h1>

            <div className='data-container'>
                <p className='data-title'>Time</p>
                <p>{time} s</p>
            </div>

            <div className='data-container'>
                <p className='data-title'>Distance</p>
                <p>{distance} m</p>
            </div>

            <div className='data-container'>
                <p className='data-title'>Speed</p>
                <p>{speed} m/s</p>
            </div>

            <div className='data-container'>
                <p className='data-title'>Acceleration</p>
                <p>{acceleration} m/s</p>
            </div>
        </div>
    );
}

export default Performance;