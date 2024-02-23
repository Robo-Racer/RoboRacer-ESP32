import React, { useState, useEffect } from 'react';
import Box from '@mui/material/Box';
import TextField from '@mui/material/TextField';
import InputAdornment from '@mui/material/InputAdornment';
import FormControl from '@mui/material/FormControl';
import Select from '@mui/material/Select';
import InputLabel from '@mui/material/InputLabel';
import MenuItem from '@mui/material/MenuItem';
import { useMemo } from 'react';
import speedProfilesData from '../data/speed_profiles.json';

// grab the saved profiles from the json to populate dropdown
function useSpeedProfiles() {
    return useMemo(() => {
        return Object.keys(speedProfilesData).map((profileName) => ({
            value: profileName,
            label: profileName,
        }));
    }, []);
}

function SpeedProfiles() {

    const speedProfiles = useSpeedProfiles();

    return (
        <Box
            component="form"
            autoComplete="off"
            sx={{ display: 'flex', flexDirection: 'column', width: 500, maxWidth: '100%' }}
        >
            {/* eventually this will send data to the robot */}
            <FormControl fullWidth >
                <InputLabel id="simple-select-helper-label" sx={{ m: 1 }}>Speed Profile</InputLabel>
                <Select
                    fullWidth
                    id="speed-profiles"
                    label="Speed Profile"
                    sx={{ m: 1 }}>
                    <MenuItem value={"custom"}>Custom</MenuItem>
                    {speedProfiles.map((profile) => (
                        <MenuItem key={profile.value} value={profile.value}>
                            {profile.label}
                        </MenuItem>
                    ))}
                </Select>

                <TextField
                    fullWidth
                    label="Speed"
                    id="speed-input"
                    sx={{ m: 1 }}
                    InputProps={{
                        endAdornment: <InputAdornment position="end">m/s</InputAdornment>,
                    }}
                />
                <TextField
                    fullWidth
                    label="Distance"
                    id="distance-input"
                    sx={{ m: 1 }}
                    InputProps={{
                        endAdornment: <InputAdornment position="end">m/s</InputAdornment>,
                    }}
                />
            </FormControl>
        </Box>
    );
}

export default SpeedProfiles;