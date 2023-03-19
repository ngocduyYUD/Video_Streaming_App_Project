import { Footer } from "../layout/Footer";
import MainFeaturedPost from "../MainFeaturedPost";
import Feature from "../Feature";
const mainFeaturedPost = {
  title: "Welcome to Video Streaming App",
  description: "This App is built using React, C and Postgres",
  image: "https://source.unsplash.com/random",
  imageText: "main image description",
  linkText: "",
};
export const Home = () => {
  return (
    <>
      <MainFeaturedPost post={mainFeaturedPost} />
      <Feature />
      <Footer />
    </>
  );
};
