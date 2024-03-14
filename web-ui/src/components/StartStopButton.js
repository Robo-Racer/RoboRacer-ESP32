import React from 'react';
import { Radio } from '@mui/material';
import { styled } from '@mui/material';

const CustomRadio = styled(Radio)(() => ({
    '& .MuiSvgIcon-root': {
        borderRadius: 0,
        width: 100,
        height: 100,
    },
    '& .MuiRadio-root': {
        borderRadius: 0,
    }
}));

function StartStopButton() {

    return (
        <div className='start-stop-container'>
            <CustomRadio />
            <CustomRadio />
        </div>
    );
}

export default StartStopButton;