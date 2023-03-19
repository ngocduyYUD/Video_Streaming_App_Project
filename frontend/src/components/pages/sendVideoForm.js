import React from "react";
import { withFormik } from "formik";
import { Button } from "@mui/material";
import axios from "axios";
import CssBaseline from "@mui/material/CssBaseline";
import TextField from "@mui/material/TextField";
import Paper from "@mui/material/Paper";
import Container from "@mui/material/Container";
import Select from "@mui/material/Select";
import MenuItem from "@mui/material/MenuItem";
export const BasicForm = (props) => {
  const [isSubmitting, setSubmitting] = React.useState(false);
  const [file, setFile] = React.useState(null);
  const [values, setValues] = React.useState({
    title: "",
    description: "",
    display_mod: "Public",
  });
  const handleChange = (e) => {
    setValues({
      ...values,
      [e.target.name]: e.target.value,
    });
  };
  const handleSubmit = (e) => {
    e.preventDefault();
    setSubmitting(true);
    const reader = new FileReader();
    reader.onload = () => {
      const fileAsBinaryString = reader.result;
      const payload = {
        type: "1",
        data: {
          filename: file.name,
          file: fileAsBinaryString,
        },
      };
      // send video
      axios
        .post("http://localhost:5000", payload, {
          headers: {
            "Content-Type": "multipart/form-data",
          },
        })
        .then((res) => {
          console.log(res);
        })
        .catch((err) => {
          console.log(err);
        })
        .then(() => {
          const metadata = {
            msg_type: "3",
            title: values.title,
            display_mod: values.display_mod === "Public" ? "true" : "false",
            thumbnail: "https://picsum.photos/200/300",
            view_number: "0",
            upload_date: "2023-03-25",
            discription: values.description,
            path_to_video: file.name,
            user_id: JSON.parse(window.localStorage.getItem("user")).id_user,
          };
          // send metadata
          console.log(metadata);
          axios
            .post("http://localhost:5500/", metadata, {
              headers: {
                "Content-Type": "application/json",
              },
            })
            .then((res) => {
              console.log(res);
              window.location = "http://localhost:3000/private/"
              setSubmitting(false);
            })
            .catch((err) => {
              console.log(err);
            });
        });
    };
    reader.readAsDataURL(file);
  };
  return (
    <Container component="main" maxWidth="lg">      <CssBaseline />      <Paper
      variant="outlined"
      sx={{
        my: { xs: 3, md: 6 },
        p: { xs: 2, md: 3 },
        mx: {
          xs: 4,
          sm: 8,
          md: 10,
          lg: 15,
        },
        width: "50vw",
      }}
    >        <form onSubmit={handleSubmit}>          <TextField
      variant="outlined"
      margin="normal"
      fullWidth
      id="title"
      label="Title"
      name="title"
      autoComplete="title"
      autoFocus
      onChange={handleChange}
      value={values.title}
    />          <TextField
          variant="outlined"
          margin="normal"
          fullWidth
          id="description"
          label="Description"
          name="description"
          autoComplete="description"
          onChange={handleChange}
          value={values.description}
        />
        <Select
          labelId="Display Mode"
          id="display_mod"
          value={values.display_mod}
          label="display_mod"
          name="display_mod"
          onChange={handleChange}
        >
          <MenuItem value={"Private"}>Private</MenuItem>
          <MenuItem value={"Public"}>Public</MenuItem>
        </Select>
        <div
          style={{
            display: "flex",
          }}
        >            <TextField
            disabled
            variant="outlined"
            margin="normal"
            fullWidth
            id="filename"
            label="File Name"
            name="filename"
            autoComplete="filename"
            value={file ? file.name : ""}
          />            <input
            style={{ display: "none" }}
            id="raised-button-file"
            type="file"
            onChange={(event) => {
              const file = event.currentTarget.files[0];
              if (file) {
                setFile(file);
              }
            }}
          />            <label htmlFor="raised-button-file">              <Button
            variant="raised"
            component="span"
            style={{ marginTop: 25, marginLeft: 20 }}
          >                Upload
              </Button>            </label>          </div>          <br />          <div
          style={{
            justifyContent: "center",
            display: "flex",
          }}
        >            <Button
          type="submit"
          variant="contained"
          style={{ paddingLeft: 15 }}
          disabled={isSubmitting || !file}
        >              Submit
            </Button>          </div>        </form>      </Paper>    </Container>);
};
export const ValidatedForm = withFormik({})(BasicForm);