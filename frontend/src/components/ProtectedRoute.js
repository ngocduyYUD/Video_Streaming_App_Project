import { Navigate } from "react-router-dom";
import { useSelector } from "react-redux";
import { Navbar } from "./layout/Navbar";
export const ProtectedRoute = ({ children }) => {
  const { user } = useSelector((state) => state.auth);
  console.log(user);
  if (!user) {
    return <Navigate to="/login" />;
  }
  return (
    <>
      <Navbar />
      {children}
    </>
  );
};
