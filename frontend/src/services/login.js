import axios from "axios";
const url = "http://localhost:5000";
export const login_req = async (user) => {
  const payload = {
    msg_type: 1,
    data: {
      username: user.username,
      password: user.password,
    },
  };
  const res = await axios.post(url, payload, {
    headers: {
      "Content-Type": "application/json",
    },
  });
  console.log(res);
  const user_id = res.data?.user_id;
  if (user_id) {
    window.localStorage.setItem("user_id", user_id);
  }
  return res.data;
};
