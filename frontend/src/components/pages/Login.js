import * as React from "react";
import Avatar from "@mui/material/Avatar";
import Button from "@mui/material/Button";
import CssBaseline from "@mui/material/CssBaseline";
import TextField from "@mui/material/TextField";
import FormControlLabel from "@mui/material/FormControlLabel";
import Checkbox from "@mui/material/Checkbox";
import Link from "@mui/material/Link";
import Grid from "@mui/material/Grid";
import Box from "@mui/material/Box";
import logo from "../../assets/streaming.png";
import Typography from "@mui/material/Typography";
import Container from "@mui/material/Container";
import { withFormik } from "formik";
import * as Yup from "yup";
import { LoginSchema } from "../validations/LoginValidation";
import { useSelector, useDispatch } from "react-redux";
import { login } from "../../redux/slices/authSlice";
import { Navigate } from "react-router-dom";
function Copyright(props) {
  return (
    <Typography
      variant="body2"
      color="text.secondary"
      align="center"
      {...props}
    >
      {"Copyright © "}
      <Link color="inherit" href="https://mui.com/">
        Your Website
      </Link>{" "}
      {new Date().getFullYear()}
      {"."}
    </Typography>
  );
}
const LoginForm = (props) => {
  const {
    values,
    touched,
    errors,
    isSubmitting,
    handleChange,
    handleBlur,
    setSubmitting,
  } = props;
  const user = useSelector((state) => state.auth.user);
  const dispatch = useDispatch();
  const handleSubmit = (e) => {
    e.preventDefault();
    setSubmitting(true);
    const params = {
      headers: { "content-type": "application/json" },
      body: JSON.stringify({
        msg_type: "5",
        name: values.username,
        pwd: values.password,
      }),
      method: "POST",
    };
    fetch("http://localhost:5500", params)
      .then((res) => res.json())
      .then((response) => {
        console.log(response);
        if (response.id_user !== "NULL") {
          dispatch(login(values));
          //if (values.remember) {
            const user = {
              username: values.username,
              password: values.password,
              id_user: response.id_user,
            }
            window.localStorage.setItem("user", JSON.stringify(user));
          //}
        }
        else
        {
          alert("Sai tai khoan hoac mat khau");
        }
        setSubmitting(false);
      });
  };
  return user ? (
    <Navigate to="/" />
  ) : (
    <Container component="main" maxWidth="xs">
      <CssBaseline />
      <Box
        sx={{
          marginTop: 8,
          display: "flex",
          flexDirection: "column",
          alignItems: "center",
        }}
      >
        <Avatar
          sx={{ m: 2, bgcolor: "logoBg.main" }}
          style={{ height: 100, width: 100 }}
        >
          <img src={logo} alt="logo" height={100} width={100} />
        </Avatar>
        <Typography component="h1" variant="h5">
          Sign in
        </Typography>
        <Box component="form" onSubmit={handleSubmit} sx={{ mt: 1 }}>
          <TextField
            margin="normal"
            fullWidth
            id="username"
            label="Username"
            name="username"
            autoComplete="username"
            autoFocus
            value={values.username}
            onChange={handleChange}
            onBlur={handleBlur}
            error={touched.username && Boolean(errors.username)}
            helperText={touched.username && errors.username}
          />
          <TextField
            margin="normal"
            fullWidth
            name="password"
            label="Password"
            type="password"
            id="password"
            autoComplete="current-password"
            value={values.password}
            onChange={handleChange}
            onBlur={handleBlur}
            error={touched.password && Boolean(errors.password)}
            helperText={touched.password && errors.password}
          />
          <FormControlLabel
            control={
              <Checkbox
                id="remember"
                name="remember"
                value="remember"
                checked={values.remember}
                onChange={handleChange}
                onBlur={handleBlur}
              />
            }
            label="Remember me"
          />
          <Button
            type="submit"
            fullWidth
            variant="contained"
            disabled={isSubmitting}
            sx={{ mt: 3, mb: 2 }}
          >
            Submit
          </Button>
          <Grid container>
            <Grid item xs></Grid>
            <Grid item>
              <Link
                href="/signup"
                variant="body2"
                style={{ textDecoration: "none" }}
              >
                {"Don't have an account? Sign Up"}
              </Link>
            </Grid>
          </Grid>
        </Box>
      </Box>
      <Copyright sx={{ mt: 8, mb: 4 }} />
    </Container>
  );
};

// validate with yup
export const Login = withFormik({
  mapPropsToValues: ({ username, password, remember }) => ({
    username: username || "",
    password: password || "",
    remember: remember || false,
  }),
  validationSchema: Yup.object().shape(LoginSchema),
})(LoginForm);
