import { createSlice } from "@reduxjs/toolkit";

const initialState = {
  mode: "light",
};

const colorModeSlice = createSlice({
  name: "colorMode",
  initialState,
  reducers: {
    changeMode: (state) => {
      state.mode = state.mode === "light" ? "dark" : "light";
    },
  },
});

export const { changeMode } = colorModeSlice.actions;
export default colorModeSlice.reducer;
