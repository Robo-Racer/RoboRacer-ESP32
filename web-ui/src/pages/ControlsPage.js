import React from 'react';
import Controls from '../components/Controls.js';
import Performance from '../components/Performance.js';
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import { useNavigate } from 'react-router-dom';

const darkTheme = createTheme({
    palette: {
        mode: 'dark',
    },
});

function ControlsPage() {
    const navigate = useNavigate();

    const devPage = () => {
        navigate("/dev")
    }

    return (
        <ThemeProvider theme={darkTheme}>
            <CssBaseline />
            <div className="App">
                <div className='header-container'>
                    <h1>RoboRacer</h1>

                    <button onClick={devPage} className='dev-button'>DEV</button>
                </div>

                <div className='outer-container'>
                    <Controls />

                    <Performance />
                </div>

            </div>
        </ThemeProvider>

    );
}

export default ControlsPage;
