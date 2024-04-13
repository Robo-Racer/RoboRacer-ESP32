import React from 'react';
import { Joystick } from 'react-joystick-component';

function DevControls() {
    const handleMove = (e) => {
        console.log(e);
    }

    return (
        <div className='dev-controls-container'>
            <Joystick
                stickColor='orange'
                baseColor='black'
                size={300}
                throttle={200}
                move={handleMove}
                className="joystick" />
        </div>
    );
}

export default DevControls;