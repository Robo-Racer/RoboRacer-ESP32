import React from "react";
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';
import Button from '@mui/material/Button';
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

                    <Button variant='outlined' sx={{ height: 40, fontSize: 18 }} onClick={controlsPage}>Back to UI</Button>
                </div>

                <div className='outer-container'>
                    <DevControls />
                </div>

            </div>
        </ThemeProvider>  
    );
}

export default Dev;