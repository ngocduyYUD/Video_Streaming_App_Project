import React from "react";
import { Grid } from "@mui/material";
import VideoCard from "../VideoCard";
import { useEffect } from "react";
import { Link } from "react-router-dom";
export const Trending = () => {
  const [videoList, setVideoList] = React.useState(null);
  useEffect(() => {
    const params = {
      headers: { "content-type": "application/json" },
      body: JSON.stringify({ msg_type: "1" }),
      method: "POST",
    };
    fetch("http://localhost:5500/", params)
      .then((res) => res.json())
      .then((res) => {
        if (res.status === "ok") setVideoList(res.data);
        console.log(res.data);
      });
  }, []);
  return (
    <Grid
      container
      spacing={2}
      style={{ paddingLeft: "4vw", paddingRight: "6vw", paddingTop: 15 }}
    >      {videoList?.map((video) => ( 
      video.display_mod === "t" &&
      <Grid item xs={12} sm={6} md={4} lg={3} key={video.video_id}>
        <Link
          to={`/video/${video.video_id}`}
          style={{ textDecoration: "none" }}
        >
          <VideoCard
            title={video.title}
            thumbnail={video.thumbnail}
            description={video.description}
          />
        </Link>
      </Grid>
    ))}
    </Grid>  );
};