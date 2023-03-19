import { ThemeProvider, createTheme } from "@mui/material/styles";
import { Routes, Route } from "react-router-dom";
import { ProtectedRoute } from "./components/ProtectedRoute";
import { Login } from "./components/pages/Login";
import { Home } from "./components/pages/Home";
import { ValidatedForm } from "./components/pages/sendVideoForm";
import { Trending } from "./components/pages/Trending";
import { CssBaseline } from "@mui/material";
import { Fragment } from "react";
import { useSelector } from "react-redux";
import { Watch } from "./components/pages/Watch";
import { SignUp } from "./components/pages/SignUp";
import { PrivateVideo } from "./components/pages/PrivateVideo";

const light = createTheme({
  palette: {
    mode: "light",
    primary: {
      main: "#556f1f",
      contrastText: "#01fff6",
    },
    secondary: {
      main: "#9c27b0",
    },
    background: {
      paper: "#ffffff",
      default: "#B7A47A",
    },
    logoBg: {
      main: "#eab676",
    },
  },
});
const dark = createTheme({
  palette: {
    mode: "dark",
    primary: {
      main: "#0dde9d",
      contrastText: "#467575",
    },
    secondary: {
      main: "#b9b537",
      contrastText: "rgba(2,0,0,0.87)",
    },
    background: {
      paper: "#584069",
      default: "#402E4C",
    },
    text: {
      primary: "#f4f7f7",
    },
    logoBg: {
      main: "#eab676",
    },
  },
});

const App = () => {
  const theme = useSelector((state) => state.colorMode.mode);
  return (
    <ThemeProvider theme={theme === "light" ? light : dark}>
      <CssBaseline />
      <Fragment>
        <Routes>
          <Route
            path="/"
            element={
              <ProtectedRoute>
                <Home />
              </ProtectedRoute>
            }
          />
          <Route path="/login" element={<Login />} />
          <Route
            path="/signup"
            element={
              <SignUp />
            }
          />

          <Route
            path="/upload"
            element={
              <ProtectedRoute>
                <ValidatedForm />
              </ProtectedRoute>
            }
          />

          <Route
            path="/video/"
            element={
              <ProtectedRoute>
                <Trending />
              </ProtectedRoute>
            }
          />

          <Route
            path="*"
            element={
              <div>
                <h1>404</h1>
              </div>
            }
          />

          <Route
            path="/video/:id"
            element={
              <ProtectedRoute>
                <Watch />
              </ProtectedRoute>
            }
          />

          <Route
            path="/private"
            element={
              <ProtectedRoute>
                <PrivateVideo />
              </ProtectedRoute>
            }
          />

        </Routes>
      </Fragment>
    </ThemeProvider>
  );
};

export default App;
