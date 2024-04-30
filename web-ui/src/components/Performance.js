import React from 'react';
import { useState } from 'react';
import graphData from '../data/graph_data.json';

import {
    XYPlot,
    LineSeries,
    XAxis,
    YAxis,
    Crosshair,
    HorizontalGridLines,
    VerticalGridLines,
    LineSeriesCanvas
} from 'react-vis';

function Performance() {
    const [time, setTime] = useState(0);
    const [distance, setDistance] = useState(0);
    const [speed, setSpeed] = useState(0);
    const [data, setData] = useState([]);

    // we will eventually want to change this so that it updates every time data is received
    setData = () => {
        setData(graphData)
    }

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

            <div className='graph-container'>
                <XYPlot height={300} width={300}>
                    <XAxis title="Time (s)" orientation='bottom' position='middle' style={{
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