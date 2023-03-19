import { configureStore } from "@reduxjs/toolkit";
import authReducer from "./slices/authSlice.js";
import colorModeReducer from "./slices/colorModeSlice.js";
export const store = configureStore({
  reducer: {
    auth: authReducer,
    colorMode: colorModeReducer,
  },
});
