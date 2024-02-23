import React from 'react';
import './App.css';
import Controls from './components/Controls.js';
import Performance from './components/Performance.js';
import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';

const darkTheme = createTheme({
  palette: {
    mode: 'dark',
  },
});

function App() {
  return (
    <ThemeProvider theme={darkTheme}>
      <CssBaseline />
      <div className="App">
        <div className='header-container'>
          <h1>RoboRacer</h1>
        </div>

        <div className='outer-container'>
          <Controls />

          <Performance />
        </div>

      </div>
    </ThemeProvider>
    
  );
}

export default App;
