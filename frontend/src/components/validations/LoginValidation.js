import * as Yup from "yup";

export const LoginSchema = {
  username: Yup.string().required("Required"),
  password: Yup.string()
    .min(4, "Password must contain at least 4 characters")
    .required("Enter your password"),
};
