import React from "react";
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import { useNavigate } from "react-router-dom";
import DevControls from "../components/DevControls";

const darkTheme = createTheme({
    palette: {
        mode: 'dark',
    },
});

function Dev() {
    const navigate = useNavigate();

    const controlsPage = () => {
        navigate('/');
    }

    return (
        <ThemeProvider theme={darkTheme}>
            <CssBaseline />
            <div className="App">
                <div className='header-container'>
                    <h1>RoboRacer Dev</h1>
                    
                    <button onClick={controlsPage} className='controls-button'>CONTROLS</button>
                </div>

                <div className='dev-outer-container'>
                    <DevControls />
                </div>

            </div>
        </ThemeProvider>  
    );
}

export default Dev;
