import React from 'react';
import { useState } from 'react';
import graphData from '../data/graph_data.json';

import {
    XYPlot,
    LineSeries,
    XAxis,
    YAxis,
    Crosshair,
} from 'react-vis';

function Performance() {
    const [time, setTime] = useState(0);
    const [distance, setDistance] = useState(0);
    const [speed, setSpeed] = useState(0);
    const [data, setData] = useState(graphData);

    return (
        <div className='performance-container'>
            <h1>Performance</h1>

            <div className='data-container'>
                <p className='time-title'>Time</p>
                <p className='time'>{time} s</p>
            </div>

            <div className='data-container'>
                <p className='distance-title'>Distance</p>
                <p className='distance'>{distance} m</p>
            </div>

            <div className='data-container'>
                <p className='speed-title'>Speed</p>
                <p className='speed'>{speed} m/s</p>
            </div>

            <div className='graph-container'>
                <XYPlot height={250} width={275}>
                    <XAxis style={{
                        text: { stroke: 'white', fontWeight: 100 }
                    }} />
                    <YAxis style={{
                        text: { stroke: 'white', fontWeight: 100 }
                    }} />

                    <Crosshair />

                    <LineSeries data={data} color='orange' />
                </XYPlot>
            </div>
        </div>
    );
}

export default Performance;